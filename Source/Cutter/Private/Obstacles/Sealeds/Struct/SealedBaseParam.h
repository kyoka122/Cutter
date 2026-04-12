#pragma once

#include "SealedBaseParam.generated.h"

USTRUCT(BlueprintType)
struct FSealedBaseParam
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "共通設定", meta = (DisplayPriority=1))
	float LifeTime = 10;
	
	UPROPERTY(EditAnywhere, Category = "共通設定", meta = (DisplayPriority=1))
	int Score = 100;
	
	/*アクティブ時に再生されるアニメーションの再生時間*/
	UPROPERTY(EditAnywhere, Category = "共通設定", meta = (DisplayPriority=1))
	float moveStartAnimationDuration = 2;
	
	/*LifeTimeが尽きるまで再生されるアニメーションの再生時間*/
	UPROPERTY(EditAnywhere, Category = "共通設定", meta = (DisplayPriority=1))
	float moveEndAnimationDuration = 3;
};