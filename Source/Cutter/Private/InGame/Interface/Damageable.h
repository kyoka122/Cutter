#pragma once

#include "CoreMinimal.h"
#include "Damageable.generated.h"

/**
 * 接触によるダメージを受けるためのインターフェース(UObject)
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 接触によるダメージを受けるためのインターフェース
 */
class CUTTER_API IDamageable
{
	GENERATED_BODY()
	
public:
	/*ダメージを与える*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InGame")
	void Damage(int value, FVector damageActorPosition);
	
};