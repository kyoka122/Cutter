#pragma once

#include "CircleMoveCutterThrowTargetParam.generated.h"

/*
 * CircleMoveCutterを投げる時の指定情報
 */
USTRUCT(BlueprintType)
struct FCircleMoveCutterThrowTargetParam
{
	GENERATED_BODY()

public:
	/*一番最初にCharacterが向くべき向き*/
	UPROPERTY(BlueprintReadWrite)
	FVector2D firstLookVec = {};
	
	/*回転最大角*/
	UPROPERTY(BlueprintReadWrite)
	FVector2D rightMaxVec = {};
	
	/*回転最小角*/
	UPROPERTY(BlueprintReadWrite)
	FVector2D leftMaxVec = {};
};