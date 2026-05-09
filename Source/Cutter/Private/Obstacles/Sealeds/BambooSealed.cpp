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
	ReStart();
}

void ABambooSealed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!_canCollisionOtherObject)
	{
		return;
	}
	CheckLifeTimeIsOver(DeltaTime);
}

void ABambooSealed::CheckLifeTimeIsOver(float deltaTime)
{
	_lifeTime -= deltaTime;
	if (!_isPlayingMoveEndAnimation && _lifeTime < _param.moveEndAnimationDuration)
	{
		PlayMoveEndAnimation();
		_isPlayingMoveEndAnimation = true;
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
	if (isExecPlayer || !_canCollisionOtherObject)
	{
		param.canRobScore = false;
		return param;
	}
	param.canRobScore = true;
	param.score = _param.Score;
	TransformCutter();
	return param;
}
