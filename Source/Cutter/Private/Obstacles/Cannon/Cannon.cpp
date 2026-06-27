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
	_targetTime -= -DeltaTime;
	if (_targetTime <= 0.f)
	{
		SpawnCannonBall();
	}
}

void ACannon::ReStart()
{
	_targetTime = targetTime;
	SetActorTickEnabled(true);
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
	if (_cannonBallPool)
	{
		ACannonBall* cannonBall = _cannonBallPool->Create(GetActorTransform());
		cannonBall->RegisterReleaseFunc([this](ACannonBall* cannonBall){Release(cannonBall);});
		cannonBall->RegisterPlayerLocation(_playerTransform);
		cannonBall->ReStart();
	}
	else UE_LOG(LogSealed, Error, TEXT("_cannonBallPoolがnullです。 %s"), *GetName());
}

void ACannon::Release(ACannonBall* cannonBall)
{
	if (_cannonBallPool)
	{
		_cannonBallPool->Release(cannonBall);
	}
	if (_releaseFunc)
	{
		_releaseFunc(this);
	}
	else UE_LOG(LogSealed, Error, TEXT("_releaseFuncがnullです。 %s"), *GetName());
}