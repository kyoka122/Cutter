#pragma once

#include "StraightYoYoThrowTargetParam.generated.h"

USTRUCT(BlueprintType)
struct FStraightYoYoThrowTargetParam
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	FVector2D firstLookVec = {};
};