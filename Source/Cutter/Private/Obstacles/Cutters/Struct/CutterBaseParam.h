#pragma once

#include "CutterBaseParam.generated.h"

/*
 * カッターパラメータのBaseクラス
 */
USTRUCT(BlueprintType)
struct FCutterBaseParam
{
	GENERATED_BODY()
	
public:
	/*このカッターが当たった時に与えるダメージ*/
	UPROPERTY(EditAnywhere, Category = "共通設定", meta = (DisplayPriority=1))
	int damage = 0;
	
	/*アクティブ時に再生されるアニメーションの再生時間*/
	UPROPERTY(EditAnywhere, Category = "共通設定", meta = (DisplayPriority=1))
	float moveStartAnimationDuration = 2.f;
};