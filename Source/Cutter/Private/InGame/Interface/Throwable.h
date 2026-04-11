#pragma once

#include "CoreMinimal.h"
#include "Throwable.generated.h"

struct FCircleMoveCutterThrowTargetParam;
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
	void StartTargeting(AActor* throwActor);
	virtual void StartTargeting_Implementation(AActor* throwActor) = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InGame")
	void Throw();
	virtual void Throw_Implementation() = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InGame")
	FVector GetLocation();
	virtual FVector GetLocation_Implementation() const = 0;
};