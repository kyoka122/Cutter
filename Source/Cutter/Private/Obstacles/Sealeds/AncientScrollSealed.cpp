#include "AncientScrollSealed.h"

#include "Cutter.h"

AAncientScrollSealed::AAncientScrollSealed()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAncientScrollSealed::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!_canOverlapOtherObject)
	{
		return;
	}
	CheckLifeTimeIsOver(DeltaSeconds);
}

FScoreRobbedParam AAncientScrollSealed::RobbedScore_Implementation(bool isExecPlayer)
{
	FScoreRobbedParam param = {};
	if (!isExecPlayer || !_canOverlapOtherObject)
	{
		param.canRobScore = false;
		return param;
	}
	
	UE_LOG(LogSealed, Log, TEXT("RobbedScore:　%s"), *GetName());
	param.canRobScore = true;
	param.score = _param.Score;
	_canOverlapOtherObject = false;
	UObject* cutterObject = Cast<UObject>(TransformCutter());
	if (IThrowable* throwableCutter = Cast<IThrowable>(cutterObject))
	{
		param.throwableCutter.SetObject(cutterObject);
		param.throwableCutter.SetInterface(throwableCutter);
	}
	return param;
}