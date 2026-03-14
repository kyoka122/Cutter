#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InGame/InGameState.h"
#include "Obstacles/DataAsset/ObstacleListDataAsset.h"
#include "Struct/ObstacleSpawnData.h"
#include "ObstacleSpawner.generated.h"

UCLASS()
class CUTTER_API AObstacleSpawner : public AActor
{
	GENERATED_BODY()

public:
	AObstacleSpawner();
	void Init(TObjectPtr<UDataTable> obstacleSpawnTable);
	void Update(const TObjectPtr<AInGameState> inGameState);
	void Spawn(const FObstacleSpawnData* nextObstacleSpawnData);

protected:
	// UPROPERTY(EditAnywhere, Category = "ステージ情報")
	// float _stageBaseHeight = {};
	
	UPROPERTY(EditAnywhere, Category = "参照設定")
	UObstacleListDataAsset* _obstacleListDataAsset = {};
	
private:
	void RegisterSpawnData(TObjectPtr<UDataTable> obstacleSpawnTable);
	
private:
	TQueue<FObstacleSpawnData*> _obstacleSpawnQueue = {};
};
