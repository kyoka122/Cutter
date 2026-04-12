#include "SealedBase.h"

#include "CutterBase.h"
#include "Application/ParamDefine.h"
#include "Components/TimelineComponent.h"
#include "Struct/SealedBaseParam.h"

ASealedBase::ASealedBase()
{
	PrimaryActorTick.bCanEverTick = true;
	_moveStartAnimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveStartAnimTimeline"));
	_moveEndAnimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveEndAnimTimeline"));
}

void ASealedBase::RegisterReleaseFunc(TFunction<void(ASealedBase* sealed)> releaseFunc)
{
	_releaseFunc = releaseFunc;
}

void ASealedBase::RegisterSpawner(TSharedPtr<ObjectPool<ACutterBase>> cutterPool)
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
	else UE_LOG(LogTemp, Error, TEXT("_cutterPoolがnullです"));
	
	if (_releaseFunc)
	{
		_releaseFunc(this);
	}
	else UE_LOG(LogTemp, Error, TEXT("_releaseFuncがnullです"));
	
	return cutter;
}

void ASealedBase::SetMeshAlphaColor(float value)
{
	if (_dynamicMaterial)
	{
		_dynamicMaterial->SetScalarParameterValue(ParamDefine::AlphaControl, value);
	}
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
	if (_dynamicMaterial)
	{
		HandleBlinkUpdate(1);
	}
	_moveStartAnimTimeline->Stop();
	_playingMoveEndAnimation = false;
	GetWorldTimerManager().ClearTimer(_endAnimationTimerHandle);
	if (_releaseFunc)
	{
		_releaseFunc(this);
	}
	else UE_LOG(LogTemp, Error, TEXT("_releaseFuncがnullです"));
}

void ASealedBase::HandleBlinkUpdate(float value)
{
	if (_dynamicMaterial)
	{
		_dynamicMaterial->SetScalarParameterValue(ParamDefine::AlphaControl, value);
	}
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