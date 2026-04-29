#pragma once

#include "CircleMoveCutterThrowParam.generated.h"

USTRUCT(BlueprintType)
struct FCircleMoveCutterThrowParam
{
	GENERATED_BODY()
	
public:
	/*回転の中心までのベクトル*/
	FVector2D toStageCenterVec2D = {};
	
	/*回転の向き。 1 or -1*/
	int32 rotateDirection = 0;
};
