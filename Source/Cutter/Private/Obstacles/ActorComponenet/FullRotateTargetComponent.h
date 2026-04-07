#pragma once

#include "CoreMinimal.h"
#include "TargetComponentBase.h"
#include "Components/ActorComponent.h"
#include "Obstacles/Struct/StraightYoYoThrowTargetParam.h"
#include "FullRotateTargetComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CUTTER_API UFullRotateTargetComponent : public UTargetComponentBase
{
	GENERATED_BODY()

public:
	UFullRotateTargetComponent();
	void RegisterParam(const FStraightYoYoThrowTargetParam& throwTargetParam);
	virtual void Init() override;

private:
	virtual void Move(const FInputActionValue& Value) override;
	virtual void Throw(const FInputActionValue& Value) override;
	
private:
	FStraightYoYoThrowTargetParam _throwTargetParam;
};
