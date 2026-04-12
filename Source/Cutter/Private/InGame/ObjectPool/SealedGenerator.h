#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utility/PoolObjectGenerator.h"
#include "SealedGenerator.generated.h"

class ASealedBase;

UCLASS(BlueprintType)
class CUTTER_API ASealedGenerator : public AActor, public PoolObjectGenerator<ASealedBase>
{
	GENERATED_BODY()

public:
	void RegisterGeneratePrefab(TSubclassOf<ASealedBase> prefab);
	void RegisterParam(const TFunction<void(ASealedBase* sealed)>& releaseFunc);
	virtual TObjectPtr<ASealedBase> Generate() override;
	virtual void Activate(TObjectPtr<ASealedBase> sealed, FTransform transform) override;
	virtual void Activate(TObjectPtr<ASealedBase> cutter) override {}
	virtual void Deactivate(TObjectPtr<ASealedBase> sealed) override;
	void SafeDeactivate(TObjectPtr<ASealedBase> sealed);

protected:
	TSubclassOf<ASealedBase> _prefab;
	TFunction<void(ASealedBase* cutter)> _releaseFunc = {};
};
