// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CutterBaseParam.h"
#include "Engine/DataAsset.h"
#include "StraightYoYoCutterCutterParam.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FStraightYoYoCutterCutterParam : public FCutterBaseParam
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = "移動パラメータ", meta = (DisplayPriority=2))
	float radianFrequency = 1.f;

	UPROPERTY(EditAnywhere, Category = "移動パラメータ")
	FVector rotateCenterPos = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "回転パラメータ", meta = (DisplayPriority=2))
	float rotateRate = 10.f;
	
	UPROPERTY(EditAnywhere, Category = "投げる時のキャラクターパラメータ", meta = (DisplayPriority=2))
	float targetingRotateSpeed = 0.f;
};

//MEMO: 使用中止
// UCLASS()
// class CUTTER_API UStraightRoundTripCutterDataAsset : public UPrimaryDataAsset
// {
// 	GENERATED_BODY()
// };
