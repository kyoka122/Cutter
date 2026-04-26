#include "BambooSealed.h"

#include "Cutter.h"
#include "Obstacles/Struct/ScoreRobbedParam.h"

ABambooSealed::ABambooSealed()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABambooSealed::BeginPlay()
{
	Super::BeginPlay();
	UStaticMeshComponent* staticMeshComponent = GetStaticMesh();
	if (!IsValid(staticMeshComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("staticMeshComponentが取得できませんでした。"));
		return;
	}
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
		if (_releaseFunc)
		{
			_releaseFunc(this);
		}
		else UE_LOG(LogSealed, Error, TEXT("_releaseFunc 実行する関数がnullです %s"), *GetName());
	}
}

FScoreRobbedParam ABambooSealed::RobbedScore_Implementation(bool isExecPlayer)
{
	UE_LOG(LogSealed, Log, TEXT("RobbedScore　%s"), *GetName());
	FScoreRobbedParam param = {};
	if (isExecPlayer)
	{
		param.canRobScore = false;
		return param;
	}
	SetActorEnableCollision(false);
	param.canRobScore = true;
	param.score = _param.Score;
	TransformCutter();
	return param;
	
}
