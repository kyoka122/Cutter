#pragma once

#include "StraightYoYoThrowTargetParam.generated.h"

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
};