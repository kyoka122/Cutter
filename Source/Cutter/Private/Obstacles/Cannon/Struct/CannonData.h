#pragma once

#include "GameplayTagContainer.h"
#include "CannonData.generated.h"

class ACannon;

USTRUCT(BlueprintType)
struct FCannonData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FGameplayTag type = {};

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACannon> cannonActor = {};
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ACannonBall> cannonBallActor = {};
};