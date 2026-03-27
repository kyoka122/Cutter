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
	
	void StartTick();
	void StopTick();
	virtual void ReStart(){}
	typedef TFunction<void(FGameplayTag,FTransform)> TransformCutterFunc;
	void RegisterTransformCutterData(int score, FGameplayTag type, TransformCutterFunc transformCutterFunc);
	
protected:
	int _score = {};
	FGameplayTag _type = {};
	TransformCutterFunc _transformCutterFunc = {};
};
