#pragma once

#include "CoreMinimal.h"
#include "Damageable.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

class CUTTER_API IDamageable
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InGame")
	void Damage(int value);
	
};