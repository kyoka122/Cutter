#include "SealedBase.h"

#include "Cutter.h"
#include "Obstacles/Cutters/CutterBase.h"
#include "Application/ParamDefine.h"
#include "Components/TimelineComponent.h"
#include "Struct/SealedBaseParam.h"

ASealedBase::ASealedBase()
{
	PrimaryActorTick.bCanEverTick = true;
	_moveStartAnimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveStartAnimTimeline"));
	_moveEndAnimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveEndAnimTimeline"));
}

void ASealedBase::RegisterReleaseFunc(const TFunction<void(ASealedBase* sealed)>& releaseFunc)
{
	_releaseFunc = releaseFunc;
}

void ASealedBase::RegisterSpawner(const TSharedPtr<ObjectPool<ACutterBase>>& cutterPool)
{
	_cutterPool = cutterPool;
}

void ASealedBase::InitTimeline(UStaticMeshComponent* staticMeshComponent)
{
	check(IsValid(staticMeshComponent));
	_dynamicMaterial = staticMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
	check(IsValid(_dynamicMaterial));
	check(IsValid(_blinkCurve));
	check(IsValid(_moveStartAnimTimeline));
	
	FOnTimelineFloat sizeUpdater;
	FOnTimelineFloat alphaUpdater;
	sizeUpdater.BindUFunction(this, "HandleSizeUpUpdate");
	alphaUpdater.BindUFunction(this, "HandleBlinkUpdate");
	_moveStartAnimTimeline->AddInterpFloat(_sizeUpCurve, sizeUpdater);
	_moveEndAnimTimeline->AddInterpFloat(_blinkCurve, alphaUpdater);
}

ACutterBase* ASealedBase::TransformCutter()
{
	ACutterBase* cutter = nullptr;
	if (_cutterPool)
	{
		cutter = _cutterPool->Create(GetActorTransform());
		cutter->ReStart();
	}
	else UE_LOG(LogSealed, Error, TEXT("_cutterPoolがnullです。 %s"), *GetName());
	
	if (_releaseFunc)
	{
		_releaseFunc(this);
	}
	else UE_LOG(LogSealed, Error, TEXT("_releaseFuncがnullです。 %s"), *GetName());
	
	return cutter;
}

void ASealedBase::SetMeshAlphaColor(float value) const
{
	if (IsValid(_dynamicMaterial))
	{
		_dynamicMaterial->SetScalarParameterValue(ParamDefine::AlphaControl, value);
	}
	else UE_LOG(LogSealed, Error, TEXT("_dynamicMaterialがnullです。 %s"), *GetName());
}

void ASealedBase::PlayMoveStartAnimation()
{
	check(IsValid(_moveStartAnimTimeline));
	_originSizeCache = GetActorScale3D();
	_moveStartAnimTimeline->SetTimelineLength(GetParam()->moveStartAnimationDuration);
	
	_moveStartAnimTimeline->PlayFromStart();
	GetWorldTimerManager().SetTimer(
		_startAnimationTimerHandle,
		[this]{OnEndMoveStartAnimation();},
		GetParam()->moveStartAnimationDuration,
		false);
}

void ASealedBase::PlayMoveEndAnimation()
{
	check(IsValid(_moveEndAnimTimeline));
	_moveStartAnimTimeline->SetTimelineLength(GetParam()->moveEndAnimationDuration);
	
	_moveEndAnimTimeline->PlayFromStart();
	GetWorldTimerManager().SetTimer(
		_endAnimationTimerHandle,
		[this]{OnEndMoveEndAnimation();},
		GetParam()->moveEndAnimationDuration,
		false);
}

void ASealedBase::OnEndMoveStartAnimation()
{
	check(IsValid(_moveStartAnimTimeline));
	HandleSizeUpUpdate(1);
	_moveStartAnimTimeline->Stop();
	GetWorldTimerManager().ClearTimer(_startAnimationTimerHandle);
	SetActorEnableCollision(true);
}

void ASealedBase::OnEndMoveEndAnimation()
{
	check(IsValid(_moveStartAnimTimeline));
	HandleBlinkUpdate(1);
	_moveStartAnimTimeline->Stop();
	_playingMoveEndAnimation = false;
	GetWorldTimerManager().ClearTimer(_endAnimationTimerHandle);
	if (_releaseFunc)
	{
		_releaseFunc(this);
	}
	else UE_LOG(LogSealed, Error, TEXT("_releaseFuncがnullです。 %s"), *GetName());
}

void ASealedBase::HandleBlinkUpdate(float value) const
{
	if (_dynamicMaterial)
	{
		_dynamicMaterial->SetScalarParameterValue(ParamDefine::AlphaControl, value);
	}
	else UE_LOG(LogSealed, Error, TEXT("_dynamicMaterialがnullです。 %s"), *GetName());
}

void ASealedBase::HandleSizeUpUpdate(float value)
{
	SetActorScale3D(_originSizeCache * value);
}

void ASealedBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(_endAnimationTimerHandle);
	GetWorldTimerManager().ClearTimer(_startAnimationTimerHandle);
}