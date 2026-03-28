#include "BambooSealed.h"

#include "Cutter.h"
#include "Struct/ScoreRobbedParam.h"

ABambooSealed::ABambooSealed()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABambooSealed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckLifeTimeIsOver(DeltaTime);
}

void ABambooSealed::ReStart()
{
	Super::ReStart();
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	_lifeTime = _param.LifeTime;
}

void ABambooSealed::CheckLifeTimeIsOver(float deltaTime)
{
	_lifeTime -= deltaTime;
	if (_lifeTime < 0.f)
	{
		if (_inactiveFunc)
		{
			_inactiveFunc();
		}
		else
		{
			UE_LOG(LogSealed, Error, TEXT("_inactiveFunc 実行する関数がnullです %s by%s"), *GetName());
		}
	}
}

FScoreRobbedParam ABambooSealed::RobbedScore_Implementation(bool isExecPlayer)
{
	FScoreRobbedParam param = {};
	if (isExecPlayer)
	{
		param.canRobScore = false;
		return param;
	}
	if (!_transformCutterFunc)
	{
		UE_LOG(LogSealed, Error, TEXT("_transformCutterFunc 実行する関数がnullです %s"), *GetName());
		param.canRobScore = false;
		return param;
	}
	SetActorEnableCollision(false);
	param.canRobScore = true;
	param.score = _param.Score;
	_transformCutterFunc(_type, GetActorTransform());
	return param;
	
}
