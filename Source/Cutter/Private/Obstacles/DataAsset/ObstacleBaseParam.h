#pragma once
#include "ObstacleBaseParam.generated.h"

USTRUCT(BlueprintType)
struct FObstacleBaseParam
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "共通設定", meta = (DisplayPriority=1))
	bool isPlayerInteractable = {};
	
	UPROPERTY(EditAnywhere, Category = "共通設定", meta = (DisplayPriority=1))
	bool isPlayerThrowable = {};
};