#pragma once
#include "SealedBaseParam.generated.h"

USTRUCT(BlueprintType)
struct FSealedBaseParam
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "共通設定", meta = (DisplayPriority=1))
	float LifeTime = {};
	
	UPROPERTY(EditAnywhere, Category = "共通設定", meta = (DisplayPriority=1))
	int Score = {};
};