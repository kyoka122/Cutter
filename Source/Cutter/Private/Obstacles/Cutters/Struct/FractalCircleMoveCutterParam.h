#pragma once

#include "CutterBaseParam.h"
#include "FractalCircleMoveCutterParam.generated.h"

USTRUCT(BlueprintType)
struct FFractalCircleMoveCutterParam : public FCutterBaseParam
{
	GENERATED_BODY()
	
public:
	/*アクタ基準で回転する回転のスピード*/
	UPROPERTY(EditAnywhere, Category = "移動パラメータ", meta = (DisplayPriority=2))
	float moveRate = 1.f;

	/*ステージ中を動き回る時のスピード*/
	UPROPERTY(EditAnywhere, Category = "回転パラメータ", meta = (DisplayPriority=2))
	float rotateRate = 10.f;
	
	/*子オブジェクト1層あたりの個数*/
	UPROPERTY(EditAnywhere, Category = "子パラメータ", meta = (DisplayPriority=2))
	int childCountPerLayer = 0;
	
	/*子オブジェクトを何層作るか*/
	UPROPERTY(EditAnywhere, Category = "子パラメータ", meta = (DisplayPriority=2))
	int depth = 1;
	
	/*1つ親のオブジェクトとの距離*/
	UPROPERTY(EditAnywhere, Category = "子パラメータ", meta = (DisplayPriority=2))
	float distanceFromParent = 10;
	
	/*子オブジェクトのサイズ縮小率*/
	UPROPERTY(EditAnywhere, Category = "子パラメータ", meta = (DisplayPriority=2))
	float sizeFactor = 0.5f;
	
	/*ターゲット時の回転速度*/
	UPROPERTY(EditAnywhere, Category = "ターゲット時パラメータ", meta = (DisplayPriority=2))
	float targetRotateSpeed = 1.f;
	
	/*ミニマップの円状の線をいくつの点で構成するか(値が大きいほど円が滑らかになる)*/
	UPROPERTY(BlueprintReadWrite)
	int32 segments = 50;
	
	/*ターゲット時にUIに表示する画像*/
	UPROPERTY(EditAnywhere, Category = "ターゲット時パラメータ", meta = (DisplayPriority=2))
	TObjectPtr<UTexture2D> looksTexture;
};
