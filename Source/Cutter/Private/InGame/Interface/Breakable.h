#pragma once

#include "CoreMinimal.h"
#include "Breakable.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI,Blueprintable)
class UBreakable : public UInterface
{
	GENERATED_BODY()
};


class CUTTER_API IBreakable
{
	GENERATED_BODY()
	
public:
	virtual void Break() = 0;
};