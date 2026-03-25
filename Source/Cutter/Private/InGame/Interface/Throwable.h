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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InGame")
	void StartTargeting();
	virtual void StartTargeting_Implementation() = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InGame")
	void Throw();
	virtual void Throw_Implementation() = 0;
};