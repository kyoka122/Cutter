#pragma once

#include "CutterBaseParam.generated.h"

USTRUCT(BlueprintType)
struct FCutterBaseParam
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "共通設定", meta = (DisplayPriority=1))
	int damage = 0;
	
	UPROPERTY(EditAnywhere, Category = "共通設定", meta = (DisplayPriority=1))
	float moveStartAnimationDuration = 2.f;
};