#pragma once

#include "CoreMinimal.h"
#include "ObstacleBaseParam.h"
#include "Engine/DataAsset.h"
#include "CircleExpandCutterDataAsset.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCircleExpandCutterParam : public FObstacleBaseParam
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "回転パラメータ", meta = (DisplayPriority=2))
	float rotateRate = 10.f;

	UPROPERTY(EditAnywhere, Category = "移動パラメータ", meta = (DisplayPriority=2))
	float moveRate = 1.f;

	UPROPERTY(EditAnywhere, Category = "移動パラメータ", meta = (DisplayPriority=2))
	FVector rotateCenterPos = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "移動パラメータ", meta = (DisplayPriority=2))
	float pitch = 10.f;
};

//MEMO: 使用中止
// UCLASS(BlueprintType, Blueprintable)
// class CUTTER_API UCircleExpandCutterDataAsset : public UPrimaryDataAsset
// {
// 	GENERATED_BODY()
// 	
// public:
// 	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
// 	FCircleExpandCutterParam param;
// };
