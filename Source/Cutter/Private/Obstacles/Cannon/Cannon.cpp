#include "Cannon.h"

#include "CannonBall.h"
#include "InGame/Interface/ActorTransform.h"
#include "Cutter.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	ReStart();
}

void ACannon::Tick(float DeltaTime)
{
	if (_isFired)
	{
		return;
	}
	_leftTime -= DeltaTime;
	if (_leftTime <= 0.f)
	{
		SpawnCannonBall();
	}
}

void ACannon::ReStart()
{
	_leftTime = targetTime;
	_isFired = false;
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
}

void ACannon::RegisterReleaseFunc(const TFunction<void(ACannon* cannon)>& releaseFunc)
{
	_releaseFunc = releaseFunc;
}

void ACannon::RegisterCannonBallSpawner(const TSharedPtr<ObjectPool<ACannonBall>>& cannonBallPool)
{
	_cannonBallPool = cannonBallPool;
}

void ACannon::RegisterPlayerLocation(const TScriptInterface<IActorTransform>& playerTransform)
{
	_playerTransform = playerTransform;
}

void ACannon::SpawnCannonBall()
{
	_isFired = true;
	if (_cannonBallPool)
	{
		ACannonBall* cannonBall = _cannonBallPool->Create(GetActorTransform());
		cannonBall->RegisterReleaseCannonFunc([this]{Release();});
		cannonBall->ReStart();
	}
	else UE_LOG(LogSealed, Error, TEXT("_cannonBallPoolがnullです。 %s"), *GetName());
}

void ACannon::Release()
{
	if (_releaseFunc)
	{
		_releaseFunc(this);
	}
	else UE_LOG(LogSealed, Error, TEXT("_releaseFuncがnullです。 %s"), *GetName());
}