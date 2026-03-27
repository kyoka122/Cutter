#include "BambooSealed.h"

#include "Struct/ScoreRobbedParam.h"

ABambooSealed::ABambooSealed()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABambooSealed::ReStart()
{
	Super::ReStart();
	SetActorEnableCollision(true);
}

FScoreRobbedParam ABambooSealed::RobbedScore_Implementation(bool isExecPlayer)
{
	FScoreRobbedParam param = {};
	if (isExecPlayer)
	{
		param.canRobScore = false;
		return param;
	}
	if (_transformCutterFunc)
	{
		SetActorEnableCollision(false);
		param.canRobScore = true;
		param.score = _score;
		_transformCutterFunc(_type, GetActorTransform());
		return param;
	}
	
	param.canRobScore = false;
	return param;
}
