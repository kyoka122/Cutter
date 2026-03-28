#pragma once

#include "CoreMinimal.h"
#include "CutterBase.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "SealedBase.generated.h"

UCLASS()
class CUTTER_API ASealedBase : public AActor
{
	GENERATED_BODY()

public:
	ASealedBase();
	
	virtual void ReStart(){}
	typedef TFunction<TObjectPtr<ACutterBase>(FGameplayTag,FTransform)> TransformCutterFunc;
	void RegisterTransformCutterData(FGameplayTag type, const TransformCutterFunc& transformCutterFunc);
	void RegisterInactiveFunc(TFunction<void()> inactiveFunc);
	
protected:
	float _lifeTime = 0.f;
	FGameplayTag _type = {};
	TransformCutterFunc _transformCutterFunc = {};
	TFunction<void()> _inactiveFunc = {};
};
