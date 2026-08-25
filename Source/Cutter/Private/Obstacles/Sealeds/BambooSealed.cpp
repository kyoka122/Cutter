#include "BambooSealed.h"

#include "Cutter.h"
#include "Obstacles/Struct/ScoreRobbedParam.h"

ABambooSealed::ABambooSealed()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABambooSealed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!_canOverlapOtherObject)
	{
		return;
	}
	CheckLifeTimeIsOver(DeltaTime);
}


FScoreRobbedParam ABambooSealed::RobbedScore_Implementation(bool isExecPlayer)
{
	FScoreRobbedParam param = {};
	if (isExecPlayer || !_canOverlapOtherObject)
	{
		param.canRobScore = false;
		return param;
	}
	
	UE_LOG(LogSealed, Log, TEXT("RobbedScore:　%s"), *GetName());
	param.canRobScore = true;
	param.score = _param.Score;
	TransformCutter();
	return param;
}
