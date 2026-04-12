#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utility/ObjectPool.h"
#include "ObstacleSpawner.generated.h"

class ACutterBase;
class UCutterListDataAsset;
class AInGameState;
class IStageShape;
struct FObstacleSpawnData;
class ASealedBase;

UCLASS()
class CUTTER_API AObstacleSpawner : public AActor
{
	GENERATED_BODY()

public:
	AObstacleSpawner();
	void Init(TObjectPtr<UDataTable> obstacleSpawnTable, const TScriptInterface<IStageShape>& stageShape,
		const TFunction<void(int)>& scoreAddFunc);
	void Update(const TObjectPtr<AInGameState> inGameState);
	void SpawnInOrder(const FObstacleSpawnData* nextObstacleSpawnData);

protected:
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TObjectPtr<UCutterListDataAsset> _cutterListDataAsset = {};

private:
	void InitGenerator(const TScriptInterface<IStageShape>& stageShape, const TFunction<void(int)>& scoreAddFunc);
	void RegisterSpawnData(TObjectPtr<UDataTable> obstacleSpawnTable);
	void SpawnSealed(const FObstacleSpawnData* nextObstacleSpawnData);

private:
	TMap<TSubclassOf<ACutterBase>, TSharedPtr<ObjectPool<ACutterBase>>> _cutterPools;
	TMap<TSubclassOf<ASealedBase>, TSharedPtr<ObjectPool<ASealedBase>>> _sealedPools;
	TQueue<FObstacleSpawnData*> _obstacleSpawnQueue = {};
};
