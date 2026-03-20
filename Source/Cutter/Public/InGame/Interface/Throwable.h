#pragma once

#include "CoreMinimal.h"
#include "Throwable.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UThrowable : public UInterface
{
	GENERATED_BODY()
};

class CUTTER_API IThrowable
{
	GENERATED_BODY()
	
public:
	virtual void OnStartTargeting() = 0;
	virtual void Throw() = 0;
};