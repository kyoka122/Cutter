#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "SealedBase.generated.h"

UCLASS()
class CUTTER_API ASealedBase : public AActor
{
	GENERATED_BODY()

public:
	ASealedBase();
	
	typedef TFunction<void(FGameplayTag,FTransform)> CutterSpawnFunc;
	void InitCutterSpawnData(int score, FGameplayTag type, CutterSpawnFunc cutterSpawnFunc);
	
protected:
	int _score = {};
	FGameplayTag _type = {};
	CutterSpawnFunc _cutterSpawnFunc = {};
};
