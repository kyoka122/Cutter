#pragma once

#include "CoreMinimal.h"
#include "Obstacles/Enum/CutterMode.h"
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
	int RobbedScore();
	virtual int RobbedScore_Implementation() = 0;
	
	virtual ECutterMode GetCurrentMode() = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InGame")
	bool IsPlayerInteractable();
	
	virtual bool IsPlayerInteractable_Implementation() = 0;
};