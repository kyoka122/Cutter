#include "AncientScrollSealed.h"

AAncientScrollSealed::AAncientScrollSealed()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAncientScrollSealed::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckLifeTimeIsOver(DeltaSeconds);
}

void AAncientScrollSealed::ReStart()
{
	Super::ReStart();
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	_lifeTime = _param.LifeTime;
}

void AAncientScrollSealed::CheckLifeTimeIsOver(float deltaTime)
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
			UE_LOG(LogTemp, Error, TEXT("_inactiveFunc 実行する関数がnullです"));
		}
	}
}

FScoreRobbedParam AAncientScrollSealed::RobbedScore_Implementation(bool isExecPlayer)
{
	FScoreRobbedParam param = {};
	if (!_transformCutterFunc)
	{
		UE_LOG(LogTemp, Error, TEXT("_transformCutterFunc 実行する関数がnullです"));
		param.canRobScore = false;
		return param;
	}
	SetActorEnableCollision(false);
	param.canRobScore = true;
	param.score = _param.Score;
	UObject* cutterObject = Cast<UObject>(_transformCutterFunc(_type, GetActorTransform()));
	if (IThrowable* throwableCutter = Cast<IThrowable>(cutterObject))
	{
		param.throwableCutter.SetObject(cutterObject);
		param.throwableCutter.SetInterface(throwableCutter);
	}
	return param;
}