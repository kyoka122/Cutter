#include "SealedBase.h"

#include "Cutter.h"
#include "Obstacles/Cutters/CutterBase.h"
#include "Application/MaterialParam.h"
#include "Components/TimelineComponent.h"
#include "Struct/SealedBaseParam.h"

ASealedBase::ASealedBase()
{
	PrimaryActorTick.bCanEverTick = true;
	_moveStartAnimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveStartAnimTimeline"));
	_moveEndAnimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveEndAnimTimeline"));
}

void ASealedBase::BeginPlay()
{
	Super::BeginPlay();
	UStaticMeshComponent* staticMeshComponent = GetMainMesh();
	if (!IsValid(staticMeshComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("_staticMeshComponentが取得できませんでした:　%s"), *GetName());
		return;
	}
	InitTimeline(staticMeshComponent);
	ReStart();
}

void ASealedBase::ReStart()
{
	_canOverlapOtherObject = false;
	_isPlayingMoveEndAnimation = false;
	_lifeTime = GetParam()->LifeTime;
	
	SetActiveOverlapComponent(true);
	PlayMoveStartAnimation();
}

void ASealedBase::RegisterReleaseFunc(const TFunction<void(ASealedBase* sealed)>& releaseFunc)
{
	_releaseFunc = releaseFunc;
}

void ASealedBase::RegisterCutterSpawner(const TSharedPtr<ObjectPool<ACutterBase>>& cutterPool)
{
	_cutterPool = cutterPool;
}

void ASealedBase::InitTimeline(UStaticMeshComponent* staticMeshComponent)
{
	check(IsValid(_moveStartAnimTimeline));
	check(IsValid(_moveEndAnimTimeline));
	
	_dynamicMaterial = staticMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
	if (!_dynamicMaterial)
	{
		UE_LOG(LogSealed, Error, TEXT("_dynamicMaterialがnullです。 %s"), *GetName());
		return;
	}
	
	FOnTimelineFloat sizeUpdater;
	FOnTimelineFloat alphaUpdater;
	sizeUpdater.BindUFunction(this, "HandleSizeUpUpdate");
	alphaUpdater.BindUFunction(this, "HandleBlinkUpdate");
	
	if (_sizeUpCurve)
	{
		_moveStartAnimTimeline->AddInterpFloat(_sizeUpCurve, sizeUpdater);
	}
	else UE_LOG(LogSealed, Error, TEXT("_sizeUpCurveがnullです。 %s"), *GetName());
	
	if (_blinkCurve)
	{
		_moveEndAnimTimeline->AddInterpFloat(_blinkCurve, alphaUpdater);
	}
	else UE_LOG(LogSealed, Error, TEXT("_blinkCurveがnullです。 %s"), *GetName());
	_originSizeCache = GetActorScale3D();
}

void ASealedBase::CheckLifeTimeIsOver(float deltaTime)
{
	_lifeTime -= deltaTime;
	if (!_isPlayingMoveEndAnimation && _lifeTime < GetParam()->moveEndAnimationDuration)
	{
		PlayMoveEndAnimation();
		_isPlayingMoveEndAnimation = true;
	}
	if (_lifeTime <= 0.f)
	{
		if (_releaseFunc)
		{
			_releaseFunc(this);
		}
		else UE_LOG(LogSealed, Error, TEXT("_releaseFunc 実行する関数がnullです %s"), *GetName());
	}
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
		_dynamicMaterial->SetScalarParameterValue(MaterialParam::AlphaControl, value);
	}
	else UE_LOG(LogSealed, Error, TEXT("_dynamicMaterialがnullです。 %s"), *GetName());
}

void ASealedBase::PlayMoveStartAnimation()
{
	if (IsValid(_moveStartAnimTimeline))
	{
		_moveStartAnimTimeline->SetTimelineLength(GetParam()->moveStartAnimationDuration);
	
		_moveStartAnimTimeline->PlayFromStart();
		GetWorldTimerManager().SetTimer(
			_startAnimationTimerHandle,
			[this]{OnEndMoveStartAnimation();},
			GetParam()->moveStartAnimationDuration,
			false);
	}
	else UE_LOG(LogSealed, Error, TEXT("_moveStartAnimTimelineがnullです。"));
}

void ASealedBase::PlayMoveEndAnimation()
{
	if (IsValid(_moveEndAnimTimeline))
	{
		_moveEndAnimTimeline->SetTimelineLength(GetParam()->moveEndAnimationDuration);
		_moveEndAnimTimeline->PlayFromStart();
		GetWorldTimerManager().SetTimer(
			_endAnimationTimerHandle,
			[this]{OnEndMoveEndAnimation();},
			GetParam()->moveEndAnimationDuration,
			false);
	}
	else UE_LOG(LogSealed, Error, TEXT("_moveEndAnimTimelineがnullです。"));
}

void ASealedBase::OnEndMoveStartAnimation()
{
	if (IsValid(_moveStartAnimTimeline))
	{
		_moveStartAnimTimeline->Stop();
	}
	else UE_LOG(LogSealed, Error, TEXT("_moveStartAnimTimelineがnullです。"));
	
	GetWorldTimerManager().ClearTimer(_startAnimationTimerHandle);
	HandleSizeUpUpdate(1);
	SetActiveOverlapComponent(false);
	_canOverlapOtherObject = true;
}

void ASealedBase::OnEndMoveEndAnimation()
{
	if (IsValid(_moveEndAnimTimeline))
	{
		_moveEndAnimTimeline->Stop();
	}
	else UE_LOG(LogSealed, Error, TEXT("_moveEndAnimTimelineがnullです。"));
	
	GetWorldTimerManager().ClearTimer(_endAnimationTimerHandle);
	HandleBlinkUpdate(1);
}

void ASealedBase::HandleBlinkUpdate(float value) const
{
	if (_dynamicMaterial)
	{
		_dynamicMaterial->SetScalarParameterValue(MaterialParam::AlphaControl, value);
	}
	else UE_LOG(LogSealed, Error, TEXT("_dynamicMaterialがnullです。 %s"), *GetName());
}

void ASealedBase::HandleSizeUpUpdate(float value)
{
	SetActorScale3D(_originSizeCache * value);
}

void ASealedBase::SetActiveOverlapComponent(bool value)
{
	if (UStaticMeshComponent* collisionMesh = GetCollisionMesh())
	{
		ECollisionEnabled::Type type = value ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision;
		collisionMesh->SetCollisionEnabled(type);
		collisionMesh->SetVisibility(value);
	}
	else UE_LOG(LogSealed, Error, TEXT("GetCollisionMesh()がnullです。 %s"), *GetName());
}

void ASealedBase::ClearAllAnimation()
{
	GetWorldTimerManager().ClearTimer(_endAnimationTimerHandle);
	GetWorldTimerManager().ClearTimer(_startAnimationTimerHandle);
	OnEndMoveStartAnimation();
	OnEndMoveEndAnimation();
}
