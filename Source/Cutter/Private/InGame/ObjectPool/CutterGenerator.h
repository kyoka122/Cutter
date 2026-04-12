#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utility/PoolObjectGenerator.h"
#include "CutterGenerator.generated.h"

class IStageShape;
class ACutterBase;

UCLASS(BlueprintType)
class CUTTER_API ACutterGenerator : public AActor, public PoolObjectGenerator<ACutterBase>
{
	GENERATED_BODY()

public:
	void RegisterGeneratePrefab(TSubclassOf<ACutterBase> prefab);
	void RegisterParam(TFunction<void(int)> scoreAddFunc, TFunction<void(ACutterBase* cutter)> releaseFunc,
		TScriptInterface<IStageShape> stageShape);
	
	virtual TObjectPtr<ACutterBase> Generate() override;
	virtual void Activate(TObjectPtr<ACutterBase> cutter, FTransform transform) override;
	virtual void Activate(TObjectPtr<ACutterBase> cutter) override {}
	virtual void Deactivate(TObjectPtr<ACutterBase> cutter) override;
	
protected:
	TSubclassOf<ACutterBase> _prefab;
	TFunction<void(int)> _scoreAddFunc = {};
	TFunction<void(ACutterBase* cutter)> _releaseFunc = {};
	TScriptInterface<IStageShape> _stageShape = {};
};
