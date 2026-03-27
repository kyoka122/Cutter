#pragma once

#include "InGame/Interface/Throwable.h"
#include "ScoreRobbedParam.generated.h"

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
