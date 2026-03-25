#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Obstacles/Struct/CutterSetData.h"
#include "CutterSpawner.generated.h"

UCLASS(Blueprintable, BlueprintType)
class CUTTER_API ACutterSpawner : public AActor
{
	GENERATED_BODY()

public:
	ACutterSpawner();
	void RegisterCutterData(TFunction<void(int)> _scoreAddFunc);
	//void Spawn(FGameplayTag type, FTransform transform);

private:
	//TSubclassOf<ACutterBase> _prefab = {};
	TArray<FCutterSetData>* _cutterSetData = {};
	TFunction<void(int)> _scoreAddFunc = {};
};
