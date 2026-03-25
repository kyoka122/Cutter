#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AutoInstance.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UAutoInstance : public UInterface
{
	GENERATED_BODY()
};

class CUTTER_API IAutoInstance
{
	GENERATED_BODY()
	
public:
	virtual void SetTag(FGameplayTag tag) = 0;
	virtual FGameplayTag GetTag() const = 0;
};