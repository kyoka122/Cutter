#pragma once

#include "CoreMinimal.h"
#include "CutterBaseParam.h"
#include "Engine/DataAsset.h"
#include "StraightYoYoCutterParam.generated.h"

/*
 *　StraightYoYoCutterで使うパラメータ構造
 */
USTRUCT(BlueprintType)
struct FStraightYoYoCutterParam : public FCutterBaseParam
{
	GENERATED_BODY()
	
public:
	/*バネ振動数*/
	UPROPERTY(EditAnywhere, Category = "移動パラメータ", meta = (DisplayPriority=2))
	float radianFrequency = 1.f;

	/*アクタ基準で回転する回転のスピード*/
	UPROPERTY(EditAnywhere, Category = "回転パラメータ", meta = (DisplayPriority=2))
	float rotateRate = 10.f;
};