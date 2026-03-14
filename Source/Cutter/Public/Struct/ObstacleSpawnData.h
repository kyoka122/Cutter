#pragma once
#include "GameplayTagContainer.h"
#include "ObstacleSpawnData.generated.h"

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