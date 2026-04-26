#pragma once

#include "CircleMoveCutterThrowParam.generated.h"

USTRUCT(BlueprintType)
struct FCircleMoveCutterThrowParam
{
	GENERATED_BODY()
	
public:
	/*投げる向き*/
	FVector2D moveVec = {};
};
