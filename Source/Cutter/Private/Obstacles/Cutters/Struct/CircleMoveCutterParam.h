#pragma once

#include "CoreMinimal.h"
#include "CutterBaseParam.h"
#include "Engine/DataAsset.h"
#include "CircleMoveCutterParam.generated.h"

/**
 * CircleMoveCutterのパラメータ構造
 */
USTRUCT(BlueprintType)
struct FCircleMoveCutterParam : public FCutterBaseParam
{
	GENERATED_BODY()
	
public:
	/*アクタ基準で回転する回転のスピード*/
	UPROPERTY(EditAnywhere, Category = "移動パラメータ", meta = (DisplayPriority=2))
	float moveRate = 1.f;

	/*ステージ中を動き回る時のスピード*/
	UPROPERTY(EditAnywhere, Category = "回転パラメータ", meta = (DisplayPriority=2))
	float rotateRate = 10.f;
	
	/*ターゲット時の回転速度*/
	UPROPERTY(EditAnywhere, Category = "ターゲット時パラメータ", meta = (DisplayPriority=2))
	float targetRotateSpeed = 1.f;
	
	/*ターゲット方向の精度。拡縮が緩やかになる*/
	UPROPERTY(EditAnywhere, Category = "ターゲット時パラメータ", meta = (DisplayPriority=2))
	float targetAccuracy = 10000.f;
	
	/*ミニマップの円状の線をいくつの点で構成するか(値が大きいほど円が滑らかになる)*/
	UPROPERTY(BlueprintReadWrite)
	int32 segments = 50;
	
	/*ターゲット時にUIに表示する画像*/
	UPROPERTY(EditAnywhere, Category = "ターゲット時パラメータ", meta = (DisplayPriority=2))
	TObjectPtr<UTexture2D> looksTexture;
};