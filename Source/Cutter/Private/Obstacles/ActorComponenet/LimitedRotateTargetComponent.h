#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "TargetComponentBase.h"
#include "Components/ActorComponent.h"
#include "Obstacles/Struct/CircleMoveCutterThrowTargetParam.h"
#include "LimitedRotateTargetComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CUTTER_API ULimitedRotateTargetComponent : public UTargetComponentBase
{
	GENERATED_BODY()

public:
	ULimitedRotateTargetComponent();
	void RegisterParam(const FCircleMoveCutterThrowTargetParam& throwTargetParam);
	virtual void Init() override;

private:
	virtual void Move(const FInputActionValue& Value) override;
	virtual void Throw(const FInputActionValue& Value) override;
	
private:
	FCircleMoveCutterThrowTargetParam _throwTargetParam;
};
