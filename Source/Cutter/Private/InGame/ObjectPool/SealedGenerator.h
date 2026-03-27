#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacles/SealedBase.h"
#include "Utility/PoolObjectGenerator.h"
#include "SealedGenerator.generated.h"

UCLASS(BlueprintType)
class CUTTER_API ASealedGenerator : public AActor, public PoolObjectGenerator<ASealedBase>
{
	GENERATED_BODY()

public:
	void RegisterGeneratePrefab(TSubclassOf<ASealedBase> prefab);
	virtual TObjectPtr<ASealedBase> Generate() override;
	virtual void Activate(TObjectPtr<ASealedBase> cutter, FTransform transform) override;
	virtual void Activate(TObjectPtr<ASealedBase> cutter) override {}
	virtual void Deactivate(TObjectPtr<ASealedBase> cutter) override;
	
	void RegisterScoreAddFunction(TFunction<void(int)>& scoreAddFunc);

protected:
	TSubclassOf<ASealedBase> _prefab;
	TFunction<void(int)> _scoreAddFunc;
};
