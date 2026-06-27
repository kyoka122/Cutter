#include "CannonBall.h"

#include "Cutter.h"
#include "Components/TimelineComponent.h"
#include "InGame/Interface/ActorTransform.h"

ACannonBall::ACannonBall()
{
	PrimaryActorTick.bCanEverTick = true;
	_moveStartAnimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveStartAnimTimeline"));
}

void ACannonBall::BeginPlay()
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

void ACannonBall::Tick(float DeltaTime)
{
	_currentTime += DeltaTime;
	FVector newPos = initVelocity * _currentTime - gravity * FMath::Pow(_currentTime, 2) / 2;
	SetActorLocation(newPos);
}

void ACannonBall::ReStart()
{
	PlayMoveStartAnimation();
	
	float initVelocityY = FMath::Sqrt(-2 * gravity.Y * maxHeight);
	FVector destination = _playerTransform->GetLocation();
	initVelocity = FVector(destination.X / duration, initVelocityY, destination.Z / duration);
	SetActorTickEnabled(true);
}

void ACannonBall::RegisterReleaseFunc(const TFunction<void(ACannonBall* cannonBall)>& releaseFunc)
{
	_releaseFunc = releaseFunc;
}

void ACannonBall::RegisterPlayerLocation(const TScriptInterface<IActorTransform>& playerTransform)
{
	_playerTransform = playerTransform;
}

void ACannonBall::InitTimeline(UStaticMeshComponent* staticMeshComponent)
{
	check(IsValid(_moveStartAnimTimeline));
	
	_dynamicMaterial = staticMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
	if (!_dynamicMaterial)
	{
		UE_LOG(LogCannon, Error, TEXT("_dynamicMaterialがnullです。 %s"), *GetName());
		return;
	}
	
	FOnTimelineFloat sizeUpdater;
	sizeUpdater.BindUFunction(this, "HandleSizeUpUpdate");
	
	if (_sizeUpCurve)
	{
		_moveStartAnimTimeline->AddInterpFloat(_sizeUpCurve, sizeUpdater);
	}
	else UE_LOG(LogCannon, Error, TEXT("_sizeUpCurveがnullです。 %s"), *GetName());
	
	_originSizeCache = GetActorScale3D();
}

void ACannonBall::PlayMoveStartAnimation()
{
	if (IsValid(_moveStartAnimTimeline))
	{
		_moveStartAnimTimeline->SetTimelineLength(moveStartAnimationDuration);
	
		_moveStartAnimTimeline->PlayFromStart();
		GetWorldTimerManager().SetTimer(
			_startAnimationTimerHandle,
			[this]{OnEndMoveStartAnimation();},
			moveStartAnimationDuration,
			false);
	}
	else UE_LOG(LogCannon, Error, TEXT("_moveStartAnimTimelineがnullです。"));
}

void ACannonBall::OnEndMoveStartAnimation()
{
	if (IsValid(_moveStartAnimTimeline))
	{
		_moveStartAnimTimeline->Stop();
	}
	else UE_LOG(LogCannon, Error, TEXT("_moveStartAnimTimelineがnullです。"));
	
	GetWorldTimerManager().ClearTimer(_startAnimationTimerHandle);
	HandleSizeUpUpdate(1);
}

void ACannonBall::HandleSizeUpUpdate(float value)
{
	SetActorScale3D(_originSizeCache * value);
}

void ACannonBall::ClearAllAnimation()
{
	GetWorldTimerManager().ClearTimer(_startAnimationTimerHandle);
	OnEndMoveStartAnimation();
}

void ACannonBall::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}