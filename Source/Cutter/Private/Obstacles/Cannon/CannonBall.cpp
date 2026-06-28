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
}

void ACannonBall::Tick(float DeltaTime)
{
	_currentTime += DeltaTime;
	FVector newPos = initPos + initVelocity * _currentTime + gravity * FMath::Pow(_currentTime, 2) / 2;
	SetActorLocation(newPos);
	
	if (_currentTime > duration + 1.5f)//MEMO: 一旦、目的地についたあと1.5秒後に消えるようにする
	{
		Release();
	}
}

void ACannonBall::ReStart()
{
	_currentTime = 0.0f;
	initPos = GetActorLocation();
	
	//MEMO: 最大maxHeightまで打ちあがって、durationの秒数をかけてプレイヤーに衝突する
	float initVelocityZ = 4 * maxHeight / duration;
	gravity.Z = - 8 * maxHeight / FMath::Pow(duration, 2);
	FVector moveVec = _playerTransform->GetLocation() - initPos;
	initVelocity = FVector(moveVec.X / duration, moveVec.Y / duration, initVelocityZ);
	
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	PlayMoveStartAnimation();
}

void ACannonBall::RegisterReleaseFunc(const TFunction<void(ACannonBall* cannonBall)>& releaseFunc)
{
	_releaseFunc = releaseFunc;
}

void ACannonBall::RegisterReleaseCannonFunc(const TFunction<void()>& cannonReleaseFunc)
{
	_cannonReleaseFunc = cannonReleaseFunc;
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
}

void ACannonBall::Release()
{
	if (_cannonReleaseFunc)
	{
		_cannonReleaseFunc();
	}
	else UE_LOG(LogSealed, Error, TEXT("_cannonReleaseFuncがnullです。 %s"), *GetName());
	
	if (_releaseFunc)
	{
		_releaseFunc(this);
	}
	else UE_LOG(LogSealed, Error, TEXT("_releaseFuncがnullです。 %s"), *GetName());
}

void ACannonBall::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ClearAllAnimation();
}