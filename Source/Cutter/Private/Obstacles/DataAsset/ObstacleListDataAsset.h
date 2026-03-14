#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Obstacles/BaseObstacle.h"
#include "ObstacleListDataAsset.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class CUTTER_API UObstacleListDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "プレハブ登録", meta=(Categories = "CutterType"))
	TMap<FGameplayTag, TSubclassOf<ABaseObstacle>> prefabs = {};
};
