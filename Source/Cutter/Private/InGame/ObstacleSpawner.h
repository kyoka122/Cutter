#pragma once

#include "CoreMinimal.h"
#include "CutterSpawner.h"
#include "GameFramework/Actor.h"
#include "InGame/InGameState.h"
#include "DataAsset/CutterListDataAsset.h"
#include "TableRow/ObstacleSpawnData.h"
#include "ObstacleSpawner.generated.h"

UCLASS()
class CUTTER_API AObstacleSpawner : public AActor
{
	GENERATED_BODY()

public:
	AObstacleSpawner();
	void Init(TObjectPtr<UDataTable> obstacleSpawnTable, TFunction<void(int)> scoreAddFunc);
	void Update(const TObjectPtr<AInGameState> inGameState);
	void Spawn(const FObstacleSpawnData* nextObstacleSpawnData);

	DECLARE_DELEGATE(CutterSpawnFunc);
protected:
	// UPROPERTY(EditAnywhere, Category = "ステージ情報")
	// float _stageBaseHeight = {};
	
	UPROPERTY(EditAnywhere, Category = "参照設定")
	UCutterListDataAsset* _cutterListDataAsset = {};
	
	//UPROPERTY(EditAnywhere, Category = "参照設定")
	
	
private:
	void RegisterSpawnData(TObjectPtr<UDataTable> obstacleSpawnTable, TFunction<void(int)> scoreAddFunc);
	void SpawnSealed(const FObstacleSpawnData* nextObstacleSpawnData);
	void SpawnCutter(FGameplayTag type, FTransform transform);

private:
	//TObjectPtr<ACutterSpawner> _cutterSpawner = {};
	TQueue<FObstacleSpawnData*> _obstacleSpawnQueue = {};
	TFunction<void(int)> _scoreAddFunc = {};
};
