#include "AncientScrollSealed.h"

#include "Cutter.h"

AAncientScrollSealed::AAncientScrollSealed()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAncientScrollSealed::BeginPlay()
{
	Super::BeginPlay();
	UStaticMeshComponent* staticMeshComponent = GetStaticMesh();
	if (!IsValid(staticMeshComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("_staticMeshComponentが取得できませんでした。"));
		return;
	}
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
		if (_releaseFunc)
		{
			_releaseFunc(this);
		}
		else UE_LOG(LogSealed, Error, TEXT("_releaseFunc 実行する関数がnullです %s"), *GetName());
	}
}

FScoreRobbedParam AAncientScrollSealed::RobbedScore_Implementation(bool isExecPlayer)
{
	FScoreRobbedParam param = {};
	SetActorEnableCollision(false);
	param.canRobScore = true;
	param.score = _param.Score;
	UObject* cutterObject = Cast<UObject>(TransformCutter());
	if (IThrowable* throwableCutter = Cast<IThrowable>(cutterObject))
	{
		param.throwableCutter.SetObject(cutterObject);
		param.throwableCutter.SetInterface(throwableCutter);
	}
	return param;
}