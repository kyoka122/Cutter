#include "ObstacleSpawner.h"
#include "TableRow/ObstacleSpawnData.h"

AObstacleSpawner::AObstacleSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AObstacleSpawner::Init(TObjectPtr<UDataTable> obstacleSpawnTable, TFunction<void(int)> scoreAddFunc)
{
	//_cutterSpawner = GetWorld()->SpawnActor<ACutterSpawner>(ACutterSpawner::StaticClass());
	RegisterSpawnData(obstacleSpawnTable, scoreAddFunc);
}

void AObstacleSpawner::RegisterSpawnData(TObjectPtr<UDataTable> obstacleSpawnTable, TFunction<void(int)> scoreAddFunc)
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

void AObstacleSpawner::Update(const TObjectPtr<AInGameState> inGameState)
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
		Spawn(nextObstacleSpawnData);
	}
}

void AObstacleSpawner::Spawn(const FObstacleSpawnData* nextObstacleSpawnData)
{
	if (nextObstacleSpawnData->type.MatchesTag(FGameplayTag::RequestGameplayTag(FName("CutterType"))))
	{
		SpawnSealed(nextObstacleSpawnData);
	}
	else
	{
		//TODO: 他のオブジェクトを作り始めたら追加
	}
}

void AObstacleSpawner::SpawnSealed(const FObstacleSpawnData* nextObstacleSpawnData)
{
	const auto* prefabSet = _cutterListDataAsset->prefabs.FindByPredicate([this, nextObstacleSpawnData](const FCutterSetData& cutterSet)
	{
		return cutterSet.type == nextObstacleSpawnData->type;
	});
	check(prefabSet);
	
	FTransform transform;
	transform.SetLocation(nextObstacleSpawnData->spawnPosition);
	
	TObjectPtr<ASealedBase> sealed = GetWorld()->SpawnActor<ASealedBase>(prefabSet->sealedModeActor, transform);
	
	sealed->InitCutterSpawnData(prefabSet->score, nextObstacleSpawnData->type, [this](FGameplayTag type, FTransform transform)
	{
		SpawnCutter(type, transform);
	});
}

void AObstacleSpawner::SpawnCutter(FGameplayTag type, FTransform transform)
{
	auto spawnCutter = _cutterListDataAsset->prefabs.FindByPredicate([type](FCutterSetData& cutterSet)
	{
		return cutterSet.type == type;
	});
	check(spawnCutter);
	UE_LOG(LogTemp, Log, TEXT("spawnCutter->type: %s"), *spawnCutter->type.ToString());
	check(spawnCutter->breakModeActor);
	
	TObjectPtr<ACutterBase> cutter = GetWorld()->SpawnActor<ACutterBase>(spawnCutter->breakModeActor, transform);
	cutter->RegisterScoreAddFunc(_scoreAddFunc);
}