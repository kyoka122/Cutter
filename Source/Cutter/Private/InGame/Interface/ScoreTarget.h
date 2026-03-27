#pragma once

#include "CoreMinimal.h"
#include "Obstacles/Struct/ScoreRobbedParam.h"
#include "ScoreTarget.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UScoreTarget : public UInterface
{
	GENERATED_BODY()
};

class CUTTER_API IScoreTarget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InGame")
	FScoreRobbedParam RobbedScore(bool isExecPlayer);
	virtual FScoreRobbedParam RobbedScore_Implementation(bool isExecPlayer) = 0;
};