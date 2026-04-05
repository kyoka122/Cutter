#pragma once

#include "StageProperty.generated.h"

class IStageShape;
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
	TScriptInterface<IStageShape> GetStageShape();
};