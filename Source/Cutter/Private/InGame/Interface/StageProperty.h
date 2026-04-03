#pragma once

#include "Camera/CameraActor.h"
#include "StageProperty.generated.h"

class UCameraComponent;

UINTERFACE(MinimalAPI,Blueprintable)
class UStageProperty : public UInterface
{
	GENERATED_BODY()
};


class CUTTER_API IStageProperty
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InGame")
	ACameraActor* GetOverViewCamera();
	virtual ACameraActor* GetOverViewCamera_Implementation() = 0;
};