#pragma once

#include "ActorTransform.generated.h"

/**
 * 位置情報を取得するインターフェース(UObject)
 */
UINTERFACE(MinimalAPI,Blueprintable)
class UActorTransform : public UInterface
{
	GENERATED_BODY()
};

/**
 * 位置情報を取得するインターフェース
 */
class CUTTER_API IActorTransform
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InGame")
	FVector GetLocation();
};