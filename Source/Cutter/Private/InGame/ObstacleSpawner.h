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

protected:
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TObjectPtr<UCutterListDataAsset> _cutterListDataAsset = {};

private:
	/*登録されている順に時間になったら生成する*/
	void SpawnInOrder(const FObstacleSpawnData* nextObstacleSpawnData);
	
	void InitGenerator(const TScriptInterface<IStageShape>& stageShape, const TFunction<void(int)>& scoreAddFunc);
	void RegisterSpawnData(const UDataTable* obstacleSpawnTable);
	void SpawnSealed(const FObstacleSpawnData* nextObstacleSpawnData);

private:
	TMap<TSubclassOf<ACutterBase>, TSharedPtr<ObjectPool<ACutterBase>>> _cutterPools;
	TMap<TSubclassOf<ASealedBase>, TSharedPtr<ObjectPool<ASealedBase>>> _sealedPools;
	
	/*オブジェクトの登録情報*/
	TQueue<FObstacleSpawnData*> _obstacleSpawnQueue = {};
};
