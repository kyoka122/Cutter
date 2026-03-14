// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObstacleBaseParam.h"
#include "Engine/DataAsset.h"
#include "StraightRoundTripCutterDataAsset.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FStraightRoundTripCutterParam : public FObstacleBaseParam
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = "移動パラメータ", meta = (DisplayPriority=2))
	float moveRate = 1.f;

	UPROPERTY(EditAnywhere, Category = "移動パラメータ")
	FVector rotateCenterPos = FVector(0.0f, 0.0f, 0.0f);

};

//MEMO: 使用中止
// UCLASS()
// class CUTTER_API UStraightRoundTripCutterDataAsset : public UPrimaryDataAsset
// {
// 	GENERATED_BODY()
// };
