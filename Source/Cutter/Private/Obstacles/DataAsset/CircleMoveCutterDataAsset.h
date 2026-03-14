#pragma once

#include "CoreMinimal.h"
#include "ObstacleBaseParam.h"
#include "Engine/DataAsset.h"
#include "CircleMoveCutterDataAsset.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCircleMoveCutterParam : public FObstacleBaseParam
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "ステージパラメータ", meta = (DisplayPriority=2))
	FVector stageCenterPos = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "ステージパラメータ", meta = (DisplayPriority=2))
	float stageSize = 4000.f;

	UPROPERTY(EditAnywhere, Category = "移動パラメータ", meta = (DisplayPriority=2))
	float moveRate = 1.f;

	UPROPERTY(EditAnywhere, Category = "回転パラメータ", meta = (DisplayPriority=2))
	float rotateRate = 10.f;
	
	UPROPERTY(EditAnywhere, Category = "寿命パラメータ", meta = (DisplayPriority=2))
	float lifeTime = 15.f;
};

//MEMO: 使用中止
// UCLASS(BlueprintType, Blueprintable)
// class CUTTER_API UCircleMoveCutterDataAsset : public UPrimaryDataAsset
// {
// 	GENERATED_BODY()
// 	
// public:
// 	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
// 	FCircleMoveCutterParam param;
// };
