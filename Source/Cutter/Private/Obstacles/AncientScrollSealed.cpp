#include "AncientScrollSealed.h"

#include "Cutter.h"

AAncientScrollSealed::AAncientScrollSealed()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAncientScrollSealed::BeginPlay()
{
	Super::BeginPlay();
	UStaticMeshComponent* staticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	InitTimeline(staticMeshComponent);
}

void AAncientScrollSealed::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckLifeTimeIsOver(DeltaSeconds);
}

void AAncientScrollSealed::ReStart()
{
	Super::ReStart();
	PlayMoveStartAnimation();
	SetActorTickEnabled(true);
	_lifeTime = _param.LifeTime;
}

void AAncientScrollSealed::CheckLifeTimeIsOver(float deltaTime)
{
	_lifeTime -= deltaTime;
	if (!_playingMoveEndAnimation && _lifeTime < _param.moveEndAnimationDuration)
	{
		PlayMoveEndAnimation();
		_playingMoveEndAnimation = true;
	}
	if (_lifeTime < 0.f)
	{
		if (_destroyFunc)
		{
			_destroyFunc();
		}
		else
		{
			UE_LOG(LogSealed, Error, TEXT("_inactiveFunc 実行する関数がnullです %s"), *GetName());
		}
	}
}

FScoreRobbedParam AAncientScrollSealed::RobbedScore_Implementation(bool isExecPlayer)
{
	FScoreRobbedParam param = {};
	if (!_transformFunc)
	{
		UE_LOG(LogSealed, Error, TEXT("_transformCutterFunc 実行する関数がnullです %s"), *GetName());
		param.canRobScore = false;
		return param;
	}
	SetActorEnableCollision(false);
	param.canRobScore = true;
	param.score = _param.Score;
	UObject* cutterObject = Cast<UObject>(_transformFunc(_type, GetActorTransform()));
	if (IThrowable* throwableCutter = Cast<IThrowable>(cutterObject))
	{
		param.throwableCutter.SetObject(cutterObject);
		param.throwableCutter.SetInterface(throwableCutter);
	}
	return param;
}