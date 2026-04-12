#pragma once

#include "InGame/Interface/Throwable.h"
#include "ScoreRobbedParam.generated.h"

/*
 *スコア取得時のパラメータ受け取り用の構造体
 */
USTRUCT(BlueprintType)
struct FScoreRobbedParam
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	bool canRobScore = false;
	
	UPROPERTY(BlueprintReadWrite)
	int score = 0;
	
	UPROPERTY(BlueprintReadWrite)
	TScriptInterface<IThrowable> throwableCutter = {};
};
