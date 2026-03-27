#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacles/CutterBase.h"
#include "Utility/PoolObjectGenerator.h"
#include "CutterGenerator.generated.h"

UCLASS(BlueprintType)
class CUTTER_API ACutterGenerator : public AActor, public PoolObjectGenerator<ACutterBase>
{
	GENERATED_BODY()

public:
	void RegisterGeneratePrefab(TSubclassOf<ACutterBase> prefab);
	virtual TObjectPtr<ACutterBase> Generate() override;
	virtual void Activate(TObjectPtr<ACutterBase> cutter, FTransform transform) override;
	virtual void Activate(TObjectPtr<ACutterBase> cutter) override {}
	virtual void Deactivate(TObjectPtr<ACutterBase> cutter) override;
	
protected:
	TSubclassOf<ACutterBase> _prefab;
};
