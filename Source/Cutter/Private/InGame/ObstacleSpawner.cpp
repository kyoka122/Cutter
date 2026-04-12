#include "ObstacleSpawner.h"

#include "Application/TagDefine.h"
#include "TableRow/ObstacleSpawnData.h"
#include "TableRow/StageEnvironmentParam.h"
#include "Utility/ObjectPool.h"

AObstacleSpawner::AObstacleSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AObstacleSpawner::Init(TObjectPtr<UDataTable> obstacleSpawnTable, TScriptInterface<IStageShape> stageShape, TFunction<void(int)> scoreAddFunc)
{
	_scoreAddFunc = scoreAddFunc;
	_stageShape = stageShape;
	InitGenerator();
	RegisterSpawnData(obstacleSpawnTable);
}

void AObstacleSpawner::InitGenerator()
{
	for (auto& setData : _cutterListDataAsset->prefabs)
	{
		if (!_cutterPools.Contains(setData.breakModeActor))
		{
			TObjectPtr<ACutterGenerator> cutterGenerator = GetWorld()->SpawnActor<ACutterGenerator>(ACutterGenerator::StaticClass());
			cutterGenerator->RegisterGeneratePrefab(setData.breakModeActor);
			auto cutterPool = MakeShared<ObjectPool<ACutterBase>>(cutterGenerator);
			cutterGenerator->RegisterParam(_scoreAddFunc, [cutterPool](ACutterBase* cutter){cutterPool->Release(cutter);}, _stageShape);
			_cutterPools.Add(setData.breakModeActor, cutterPool);
		}
	}
	for (auto& setData : _cutterListDataAsset->prefabs)
	{
		if (!_sealedPools.Contains(setData.sealedModeActor))
		{
			TObjectPtr<ASealedGenerator> sealedGenerator = GetWorld()->SpawnActor<ASealedGenerator>(ASealedGenerator::StaticClass());
			sealedGenerator->RegisterGeneratePrefab(setData.sealedModeActor);
			auto sealedPool = MakeShared<ObjectPool<ASealedBase>>(sealedGenerator);
			sealedGenerator->RegisterParam([sealedPool](ASealedBase* sealed){sealedPool->Release(sealed);});
			_sealedPools.Add(setData.sealedModeActor, sealedPool);
		}
	} 
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
		SpawnInOrder(nextObstacleSpawnData);
	}
}

void AObstacleSpawner::SpawnInOrder(const FObstacleSpawnData* nextObstacleSpawnData)
{
	if (nextObstacleSpawnData->type.MatchesTag(FGameplayTag::RequestGameplayTag(TagDefine::CutterType)))
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
	FCutterSetData* spawnPrefabSet = _cutterListDataAsset->prefabs.FindByPredicate([this, nextObstacleSpawnData](const FCutterSetData& cutterSet)
	{
		return cutterSet.type == nextObstacleSpawnData->type;
	});
	check(spawnPrefabSet);
	
	FTransform transform;
	transform.SetLocation(nextObstacleSpawnData->spawnPosition);
	
	TSharedPtr<ObjectPool<ASealedBase>> sealedPool = _sealedPools[spawnPrefabSet->sealedModeActor];
	TSharedPtr<ObjectPool<ACutterBase>> cutterPool = _cutterPools[spawnPrefabSet->breakModeActor];
	
	TObjectPtr<ASealedBase> sealed = sealedPool->Create(transform);
	sealed->RegisterSpawner(cutterPool);
	
	sealed->ReStart();
}