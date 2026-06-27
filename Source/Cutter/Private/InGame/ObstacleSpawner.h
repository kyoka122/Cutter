#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacles/Cannon/Struct/CannonData.h"
#include "Obstacles/Struct/CutterSetData.h"
#include "Utility/ObjectPool.h"
#include "ObstacleSpawner.generated.h"

class ACannon;
class UCannonListDataAsset;
class ACutterBase;
class UCutterListDataAsset;
class AInGameState;
class IStageShape;
struct FObstacleSpawnData;
class ASealedBase;

/*
 * 汎用スポナークラス
 */
UCLASS()
class CUTTER_API AObstacleSpawner : public AActor
{
	GENERATED_BODY()

public:
	AObstacleSpawner();
	void Init(const UDataTable* obstacleSpawnTable, const TScriptInterface<IStageShape>& stageShape,const TFunction<void(int)>& scoreAddFunc);
	
	/*更新(登録されているオブジェクトの生成時間になれば生成)*/
	void Update(const AInGameState* inGameState);

	/*今盤面に存在する全てのオブジェクトを取得する*/
	TArray<AActor*> GetCurrentUsingObstacles();
	
protected:
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TObjectPtr<UCutterListDataAsset> _cutterListDataAsset = {};
	
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TObjectPtr<UCannonListDataAsset> _cannonListDataAsset = {};

private:
	/*登録されている順に時間になったら生成する*/
	bool TrySpawnCutterByTime(const FObstacleSpawnData* nextObstacleSpawnData, float leftTime);
	void InitGenerator(const TScriptInterface<IStageShape>& stageShape, const TFunction<void(int)>& scoreAddFunc);
	void RegisterSpawnData(const UDataTable* obstacleSpawnTable);
	void SpawnSealed(const FObstacleSpawnData* nextObstacleSpawnData, const FCutterSetData* spawnPrefabSet);
	bool TrySpawnCannonByTime(const FObstacleSpawnData* nextObstacleSpawnData, float leftTime);
	void SpawnCannon(const FObstacleSpawnData* nextObstacleSpawnData);

private:
	TMap<TSubclassOf<ACutterBase>, TSharedPtr<ObjectPool<ACutterBase>>> _cutterPools;
	TMap<TSubclassOf<ASealedBase>, TSharedPtr<ObjectPool<ASealedBase>>> _sealedPools;
	TSharedPtr<ObjectPool<ACannon>> _cannonPool;
	
	/*生成するオブジェクトの情報*/
	TQueue<FObstacleSpawnData*> _obstacleSpawnQueue = {};
};
