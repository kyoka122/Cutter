#pragma once

#include "StraightYoYoThrowParam.generated.h"

USTRUCT(BlueprintType)
struct FStraightYoYoThrowParam
{
	GENERATED_BODY()
	
public:
	/*投げる向き*/
	FVector2D moveVec = {};
};
