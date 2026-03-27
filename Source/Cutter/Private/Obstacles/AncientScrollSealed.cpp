#include "AncientScrollSealed.h"

AAncientScrollSealed::AAncientScrollSealed()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAncientScrollSealed::ReStart()
{
	Super::ReStart();
	SetActorEnableCollision(true);
}

FScoreRobbedParam AAncientScrollSealed::RobbedScore_Implementation(bool isExecPlayer)
{
	FScoreRobbedParam param = {};
	if (_transformCutterFunc)
	{
		SetActorEnableCollision(false);
		param.canRobScore = true;
		param.score = _score;
		UObject* cutterObject = Cast<UObject>(_transformCutterFunc(_type, GetActorTransform()));
		if (IThrowable* throwableCutter = Cast<IThrowable>(cutterObject))
		{
			param.throwableCutter.SetObject(cutterObject);
			param.throwableCutter.SetInterface(throwableCutter);
		}
		return param;
	}
	
	param.canRobScore = false;
	return param;
}