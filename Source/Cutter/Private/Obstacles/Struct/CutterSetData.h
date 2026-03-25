#pragma once
#include "Obstacles/CutterBase.h"
#include "Obstacles/SealedBase.h"
#include "CutterSetData.generated.h"

USTRUCT(BlueprintType)
struct FCutterSetData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FGameplayTag type = {};
	
	UPROPERTY(EditAnywhere)
	int score = 0;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASealedBase> sealedModeActor = {};
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ACutterBase> breakModeActor = {};
};