#include "ObstacleSpawner.h"

#include "InGameState.h"
#include "Application/TagDefine.h"
#include "ObjectPool/CutterGenerator.h"
#include "ObjectPool/SealedGenerator.h"
#include "Obstacles/DataAsset/CutterListDataAsset.h"
#include "TableRow/ObstacleSpawnData.h"
#include "Utility/ObjectPool.h"

AObstacleSpawner::AObstacleSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AObstacleSpawner::Init(const UDataTable* obstacleSpawnTable, const TScriptInterface<IStageShape>& stageShape, const TFunction<void(int)>& scoreAddFunc)
{
	InitGenerator(stageShape, scoreAddFunc);
	RegisterSpawnData(obstacleSpawnTable);
}

void AObstacleSpawner::InitGenerator(const TScriptInterface<IStageShape>& stageShape, const TFunction<void(int)>& scoreAddFunc)
{
	for (auto& setData : _cutterListDataAsset->prefabs)
	{
		if (!_cutterPools.Contains(setData.cutterModeActor))
		{
			ACutterGenerator* cutterGenerator = GetWorld()->SpawnActor<ACutterGenerator>(ACutterGenerator::StaticClass());
			cutterGenerator->RegisterGeneratePrefab(setData.cutterModeActor);
			auto cutterPool = MakeShared<ObjectPool<ACutterBase>>(cutterGenerator);
			cutterGenerator->RegisterParam(scoreAddFunc, [cutterPool](ACutterBase* cutter){cutterPool->Release(cutter);}, stageShape);
			_cutterPools.Add(setData.cutterModeActor, cutterPool);
		}
	}
	for (auto& setData : _cutterListDataAsset->prefabs)
	{
		if (!_sealedPools.Contains(setData.sealedModeActor))
		{
			ASealedGenerator* sealedGenerator = GetWorld()->SpawnActor<ASealedGenerator>(ASealedGenerator::StaticClass());
			sealedGenerator->RegisterGeneratePrefab(setData.sealedModeActor);
			auto sealedPool = MakeShared<ObjectPool<ASealedBase>>(sealedGenerator);
			sealedGenerator->RegisterParam([sealedPool](ASealedBase* sealed){sealedPool->Release(sealed);});
			_sealedPools.Add(setData.sealedModeActor, sealedPool);
		}
	} 
}

void AObstacleSpawner::RegisterSpawnData(const UDataTable* obstacleSpawnTable)
{
	static const FString contextString = FString::Printf(TEXT("オブジェクト生成リスト読み込み失敗: "));
	TArray<FObstacleSpawnData*> obstacleSpawnData;
	obstacleSpawnTable->GetAllRows<FObstacleSpawnData>(contextString, obstacleSpawnData);

	for (const auto& row : obstacleSpawnData)
	{
		if (row)
		{
			_obstacleSpawnQueue.Enqueue(row);
		}
		else UE_LOG(LogTemp, Error, TEXT("不正なRowがあります"));
		
	}
}

void AObstacleSpawner::Update(const AInGameState* inGameState)
{
	float leftTime = inGameState->GetInitLimitTime() - inGameState->GetLimitTime();
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
	TSharedPtr<ObjectPool<ACutterBase>> cutterPool = _cutterPools[spawnPrefabSet->cutterModeActor];
	
	TObjectPtr<ASealedBase> sealed = sealedPool->Create(transform);
	sealed->RegisterCutterSpawner(cutterPool);
	
	sealed->ReStart();
}

TArray<AActor*> AObstacleSpawner::GetCurrentUsingObstacles()
{
	TArray<AActor*> usingObstacles;
	usingObstacles.Reserve(_cutterPools.Num() + _sealedPools.Num());//メモリ確保。Poolそれぞれに1ずつ以上ぐらいはあるはず。（どちらにせよすぐに使用しなくなるので無駄分は気にしない）
	for (auto& cutterPool : _cutterPools)
	{
		usingObstacles.Append(cutterPool.Value->GetCurrentUsingObject());
	}
	
	for (auto& sealedPool : _sealedPools)
	{
		usingObstacles.Append(sealedPool.Value->GetCurrentUsingObject());
	}
	
	return usingObstacles;
}