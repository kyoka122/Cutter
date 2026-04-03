#pragma once
#include "CutterThrowParam.generated.h"

USTRUCT(BlueprintType)
struct FCutterThrowTargetParam
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FVector2D firstLookVec = {};
	
	UPROPERTY(BlueprintReadWrite)
	FVector2D rightMaxVec = {};
	
	UPROPERTY(BlueprintReadWrite)
	FVector2D leftMaxVec = {};
	
	UPROPERTY(BlueprintReadWrite)
	float targetingRotateSpeed = 0.f;
};