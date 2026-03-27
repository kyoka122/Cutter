#pragma once

#include "CoreMinimal.h"
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

private:
	TArray<FCutterSetData>* _cutterSetData = {};
	TFunction<void(int)> _scoreAddFunc = {};
};
