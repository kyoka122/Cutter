#pragma once

#include "CoreMinimal.h"
#include "CutterBaseParam.h"
#include "Engine/DataAsset.h"
#include "CircleExpandCutterParam.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCircleExpandCutterParam : public FCutterBaseParam
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "回転パラメータ", meta = (DisplayPriority=2))
	float rotateRate = 10.f;

	UPROPERTY(EditAnywhere, Category = "移動パラメータ", meta = (DisplayPriority=2))
	float moveRate = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "移動パラメータ", meta = (DisplayPriority=2))
	float pitch = 10.f;
};