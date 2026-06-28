#include "ObstacleSpawner.h"

#include "InGameState.h"
#include "Application/TagDefine.h"
#include "Interface/ActorTransform.h"
#include "ObjectPool/CannonBallGenerator.h"
#include "ObjectPool/CannonGenerator.h"
#include "ObjectPool/CutterGenerator.h"
#include "ObjectPool/SealedGenerator.h"
#include "Obstacles/Cannon/Cannon.h"
#include "Obstacles/Cannon/Struct/CannonData.h"
#include "Obstacles/DataAsset/CannonListDataAsset.h"
#include "Obstacles/DataAsset/CutterListDataAsset.h"
#include "Obstacles/Sealeds/Struct/SealedBaseParam.h"
#include "TableRow/ObstacleSpawnData.h"
#include "Utility/ObjectPool.h"

class ACannonBallGenerator;

AObstacleSpawner::AObstacleSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AObstacleSpawner::Init(const UDataTable* obstacleSpawnTable, const TScriptInterface<IStageShape>& stageShape,
	const TFunction<void(int)>& scoreAddFunc, const TScriptInterface<IActorTransform>& playerTransform)
{
	InitGenerator(stageShape, scoreAddFunc, playerTransform);
	RegisterSpawnData(obstacleSpawnTable);
}

void AObstacleSpawner::InitGenerator(const TScriptInterface<IStageShape>& stageShape, const TFunction<void(int)>& scoreAddFunc,
	const TScriptInterface<IActorTransform>& playerTransform)
{
	_playerTransform = playerTransform;
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
	check(playerTransform)
	ACannonGenerator* cannonGenerator = GetWorld()->SpawnActor<ACannonGenerator>(ACannonGenerator::StaticClass());
	cannonGenerator->RegisterGeneratePrefab(_cannonListDataAsset->prefab.cannonActor);
	auto cannonPool = MakeShared<ObjectPool<ACannon>>(cannonGenerator);
	cannonGenerator->RegisterParam(scoreAddFunc, [cannonPool](ACannon* cannon){cannonPool->Release(cannon);}, playerTransform);
	_cannonPool = cannonPool;
	
	ACannonBallGenerator* cannonBallGenerator = GetWorld()->SpawnActor<ACannonBallGenerator>(ACannonBallGenerator::StaticClass());
	cannonBallGenerator->RegisterGeneratePrefab(_cannonListDataAsset->prefab.cannonBallActor);
	auto cannonBallPool = MakeShared<ObjectPool<ACannonBall>>(cannonBallGenerator);
	cannonBallGenerator->RegisterParam(scoreAddFunc, [cannonBallPool](ACannonBall* cannonBall){cannonBallPool->Release(cannonBall);}, playerTransform);
	_cannonBallPool = cannonBallPool;
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
	
	//TODO: ここの処理を基準にクラス化する
	if (nextObstacleSpawnData->type.MatchesTag(FGameplayTag::RequestGameplayTag(TagDefine::CutterType)))
	{
		if (TrySpawnCutterByTime(nextObstacleSpawnData, leftTime))
		{
			FObstacleSpawnData* tmp;
			_obstacleSpawnQueue.Dequeue(tmp);
		}
	}
	else if (nextObstacleSpawnData->type.MatchesTag(FGameplayTag::RequestGameplayTag(TagDefine::CannonType)))
	{
		if (TrySpawnCannonByTime(nextObstacleSpawnData, leftTime))
		{
			FObstacleSpawnData* tmp;
			_obstacleSpawnQueue.Dequeue(tmp);
		}
	}
}

bool AObstacleSpawner::TrySpawnCutterByTime(const FObstacleSpawnData* nextObstacleSpawnData, const float leftTime)
{
	FCutterSetData* spawnPrefabSet = _cutterListDataAsset->prefabs.FindByPredicate([this, nextObstacleSpawnData](const FCutterSetData& cutterSet)
	{
		return cutterSet.type == nextObstacleSpawnData->type;
	});
	
	float anticTime = spawnPrefabSet->sealedModeActor->GetDefaultObject<ASealedBase>()->GetParam()->moveStartAnimationDuration;
	if ((nextObstacleSpawnData->spawnTime - anticTime) <= leftTime)
	{
		SpawnSealed(nextObstacleSpawnData, spawnPrefabSet);
		return true;
	}
	return false;
}

void AObstacleSpawner::SpawnSealed(const FObstacleSpawnData* nextObstacleSpawnData, const FCutterSetData* spawnPrefabSet)
{
	FTransform transform;
	transform.SetLocation(nextObstacleSpawnData->spawnPosition);
	
	TSharedPtr<ObjectPool<ASealedBase>> sealedPool = _sealedPools[spawnPrefabSet->sealedModeActor];
	TSharedPtr<ObjectPool<ACutterBase>> cutterPool = _cutterPools[spawnPrefabSet->cutterModeActor];
	
	TObjectPtr<ASealedBase> sealed = sealedPool->Create(transform);
	sealed->RegisterCutterSpawner(cutterPool);
	sealed->ReStart();
}

bool AObstacleSpawner::TrySpawnCannonByTime(const FObstacleSpawnData* nextObstacleSpawnData, const float leftTime) const
{
	if (nextObstacleSpawnData->spawnTime <= leftTime)
	{
		SpawnCannon(nextObstacleSpawnData);
		return true;
	}
	return false;
}

void AObstacleSpawner::SpawnCannon(const FObstacleSpawnData* nextObstacleSpawnData) const
{
	FTransform transform;
	transform.SetLocation(nextObstacleSpawnData->spawnPosition + FVector::UpVector * _playerTransform->GetLocation());
	
	TObjectPtr<ACannon> cannon = _cannonPool->Create(transform);
	cannon->RegisterCannonBallSpawner(_cannonBallPool);
	cannon->ReStart();
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
	
	usingObstacles.Append(_cannonPool->GetCurrentUsingObject());
	
	return usingObstacles;
}

TArray<AActor*> AObstacleSpawner::GetCurrentUsingBamboos()
{
	TArray<AActor*> usingObstacles;
	usingObstacles.Reserve( _sealedPools.Num());
	
	FCutterSetData* bambooPrefabSet = _cutterListDataAsset->prefabs.FindByPredicate([this](const FCutterSetData& cutterSet)
	{
		FGameplayTag tag = cutterSet.sealedModeActor->GetDefaultObject<ASealedBase>()->GetTag();
		return tag.MatchesTag(FGameplayTag::RequestGameplayTag(TagDefine::Bamboo));
	});
	
	if (bambooPrefabSet)
	{
		if (_sealedPools.Contains(bambooPrefabSet->sealedModeActor))
		{
			usingObstacles.Append(_sealedPools[bambooPrefabSet->sealedModeActor]->GetCurrentUsingObject());
		}
	}else UE_LOG(LogTemp, Error, TEXT("bambooPrefabSetがnullです。"));
	
	
	return usingObstacles;
}