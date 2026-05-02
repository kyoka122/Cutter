#pragma once

#include "CircleMoveCutterThrowTargetParam.generated.h"

class IStageShape;
/*
 * CircleMoveCutterを投げる時の指定情報
 */
USTRUCT(BlueprintType)
struct FCircleMoveCutterThrowTargetParam
{
	GENERATED_BODY()

public:
	/*投げる場所*/
	UPROPERTY(BlueprintReadWrite)
	FVector2D cutterPos = {};
	
	/*ターゲット時の回転速度*/
	UPROPERTY(BlueprintReadWrite)
	float rotateSpeed = 0;
	
	/*ターゲット方向の精度。拡縮が緩やかになる*/
	UPROPERTY(BlueprintReadWrite)
	float accuracy = 0;
	
	/*ミニマップの円状の線をいくつの点で構成するか(値が大きいほど円が滑らかになる)*/
	UPROPERTY(BlueprintReadWrite)
	int32 segments = 0;
	
	/*ステージ情報*/
	UPROPERTY(BlueprintReadWrite)
	TScriptInterface<IStageShape> stageShape = {};
	
	/*ターゲット時にUIに表示する画像*/
	UPROPERTY() TObjectPtr<UTexture2D> looksTexture = {};
};