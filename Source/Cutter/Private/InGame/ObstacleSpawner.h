#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InGame/InGameState.h"
#include "DataAsset/CutterListDataAsset.h"
#include "ObjectPool/CutterGenerator.h"
#include "ObjectPool/SealedGenerator.h"
#include "TableRow/ObstacleSpawnData.h"
#include "TableRow/StageEnvironmentParam.h"
#include "Utility/ObjectPool.h"
#include "ObstacleSpawner.generated.h"

UCLASS()
class CUTTER_API AObstacleSpawner : public AActor
{
	GENERATED_BODY()

public:
	AObstacleSpawner();
	void Init(TObjectPtr<UDataTable> obstacleSpawnTable, TScriptInterface<IStageShape> stageShape, TFunction<void(int)> scoreAddFunc);
	void Update(const TObjectPtr<AInGameState> inGameState);
	void SpawnInOrder(const FObstacleSpawnData* nextObstacleSpawnData);

protected:
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TObjectPtr<UCutterListDataAsset> _cutterListDataAsset = {};

private:
	void InitGenerator();
	void RegisterSpawnData(TObjectPtr<UDataTable> obstacleSpawnTable);
	void SpawnSealed(const FObstacleSpawnData* nextObstacleSpawnData);
	TObjectPtr<ACutterBase> SpawnCutter(FGameplayTag type, const FTransform& transform);

private:
	TMap<TSubclassOf<ACutterBase>, TSharedPtr<ObjectPool<ACutterBase>>> _cutterPools;
	TMap<TSubclassOf<ASealedBase>, TSharedPtr<ObjectPool<ASealedBase>>> _sealedPools;
	TQueue<FObstacleSpawnData*> _obstacleSpawnQueue = {};
	TScriptInterface<IStageShape> _stageShape = {};
	TFunction<void(int)> _scoreAddFunc = {};
};
