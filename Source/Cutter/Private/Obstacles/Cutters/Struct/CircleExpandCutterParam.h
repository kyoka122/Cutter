#pragma once

#include "CoreMinimal.h"
#include "CutterBaseParam.h"
#include "CircleExpandCutterParam.generated.h"

/**
 * CircleExpandCutterのパラメータ構造
 */
USTRUCT(BlueprintType)
struct FCircleExpandCutterParam : public FCutterBaseParam
{
	GENERATED_BODY()
	
public:
	/*アクタ基準で回転する回転のスピード*/
	UPROPERTY(EditAnywhere, Category = "回転パラメータ", meta = (DisplayPriority=2))
	float rotateRate = 10.f;

	/*ステージ中を動き回る時のスピード*/
	UPROPERTY(EditAnywhere, Category = "移動パラメータ", meta = (DisplayPriority=2))
	float moveRate = 1.f;
	
	/*ステージ中を回転する時の回転半径基準値*/
	UPROPERTY(EditAnywhere, Category = "移動パラメータ", meta = (DisplayPriority=2))
	float pitch = 10.f;
};