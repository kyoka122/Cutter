#include "BambooSealed.h"

#include "Cutter.h"
#include "Struct/ScoreRobbedParam.h"

ABambooSealed::ABambooSealed()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABambooSealed::BeginPlay()
{
	Super::BeginPlay();
	UStaticMeshComponent* staticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	InitTimeline(staticMeshComponent);
}

void ABambooSealed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckLifeTimeIsOver(DeltaTime);
}

void ABambooSealed::ReStart()
{
	Super::ReStart();
	PlayMoveStartAnimation();
	SetActorTickEnabled(true);
	_lifeTime = _param.LifeTime;
}

void ABambooSealed::CheckLifeTimeIsOver(float deltaTime)
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

FScoreRobbedParam ABambooSealed::RobbedScore_Implementation(bool isExecPlayer)
{
	UE_LOG(LogTemp, Log, TEXT("RobbedScore_Implementation"));
	FScoreRobbedParam param = {};
	if (isExecPlayer)
	{
		param.canRobScore = false;
		return param;
	}
	if (!_transformFunc)
	{
		UE_LOG(LogSealed, Error, TEXT("_transformCutterFunc 実行する関数がnullです %s"), *GetName());
		param.canRobScore = false;
		return param;
	}
	SetActorEnableCollision(false);
	param.canRobScore = true;
	param.score = _param.Score;
	_transformFunc(_type, GetActorTransform());
	return param;
	
}
