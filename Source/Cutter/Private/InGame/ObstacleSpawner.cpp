#include "ObstacleSpawner.h"

#include "InGameMode.h"
#include "Obstacles/CutterBase.h"
#include "Struct/ObstacleSpawnData.h"

AObstacleSpawner::AObstacleSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AObstacleSpawner::Init(TObjectPtr<UDataTable> obstacleSpawnTable)
{
	RegisterSpawnData(obstacleSpawnTable);
}

void AObstacleSpawner::RegisterSpawnData(TObjectPtr<UDataTable> obstacleSpawnTable)
{
	static const FString contextString = FString::Printf(TEXT("オブジェクト生成リスト読み込み失敗: "));
	TArray<FObstacleSpawnData*> obstacleSpawnData;
	obstacleSpawnTable->GetAllRows<FObstacleSpawnData>(contextString, obstacleSpawnData);

	for (const auto& row : obstacleSpawnData)
	{
		if (!row)
		{
			UE_LOG(LogTemp, Log, TEXT("不正なRowがあります"));
		}
		_obstacleSpawnQueue.Enqueue(row);
	}
}

void AObstacleSpawner::Update(const TObjectPtr<AInGameState> inGameState, TFunction<void(int)> scoreAddFunc)
{
	float leftTime = inGameState->GetInitLimitTime()-inGameState->GetLimitTime();
	if (_obstacleSpawnQueue.IsEmpty())
	{
		return;
	}
	FObstacleSpawnData* nextObstacleSpawnData = *_obstacleSpawnQueue.Peek();
	
	if (nextObstacleSpawnData->spawnTime <= leftTime)
	{
		FObstacleSpawnData* tmp;
		_obstacleSpawnQueue.Dequeue(tmp);
		Spawn(nextObstacleSpawnData, scoreAddFunc);
	}
}

void AObstacleSpawner::Spawn(const FObstacleSpawnData* nextObstacleSpawnData, TFunction<void(int)> scoreAddFunc)
{
	TSubclassOf<ACutterBase> cutterPrefab = *_obstacleListDataAsset->prefabs.Find(nextObstacleSpawnData->type);
	check(IsValid(cutterPrefab));
	FTransform transform;
	transform.SetLocation(nextObstacleSpawnData->spawnPosition);
	TObjectPtr<ACutterBase> cutter = GetWorld()->SpawnActor<ACutterBase>(cutterPrefab, transform);
	//FVector spawnPosition = nextObstacleSpawnData->spawnPosition;
	//obstacle->SetActorLocation(FVector(spawnPosition.X, spawnPosition.Y, _stageBaseHeight + spawnPosition.Z));
	cutter->RegisterAddScoreFunc(scoreAddFunc);
}

