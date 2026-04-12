#pragma once

#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "ObstacleSpawnData.generated.h"

/*
 *時間経過で生成するオブジェクトを登録するためのテーブル
 */
USTRUCT(BlueprintType)
struct FObstacleSpawnData: public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FGameplayTag type = {};
	
	UPROPERTY(EditAnywhere)
	float spawnTime = 0;
	
	UPROPERTY(EditAnywhere)
	FVector spawnPosition = {};
};