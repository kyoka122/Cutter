#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AutoInstance.generated.h"

/**
 * 汎用SpawnerでSpawnするためのインターフェース
 */
UINTERFACE(MinimalAPI)
class UAutoInstance : public UInterface
{
	GENERATED_BODY()
};

class CUTTER_API IAutoInstance
{
	GENERATED_BODY()
	
	//TODO: 実装途中
public:
	virtual void SetTag(FGameplayTag& tag) = 0;
	virtual FGameplayTag& GetTag() const = 0;
};