#pragma once

#include "StraightYoYoThrowTargetParam.generated.h"

class IStageShape;
/*
 * StraightYoYoを投げる時の指定情報
 */
USTRUCT(BlueprintType)
struct FStraightYoYoThrowTargetParam
{
	GENERATED_BODY()
	
public:
	/*一番最初にCharacterが向くべき向き*/
	UPROPERTY(BlueprintReadWrite)
	FVector2D firstLookVec = {};
	
	/*投げる場所*/
	UPROPERTY(BlueprintReadWrite)
	FVector2D cutterPos = {};
	
	/*ターゲット時の回転速度*/
	UPROPERTY(BlueprintReadWrite)
	float rotateSpeed = 1.f;
	
	/*ステージ情報*/
	UPROPERTY(BlueprintReadWrite)
	TScriptInterface<IStageShape> stageShape = {};
};