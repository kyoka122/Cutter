#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "TargetComponentBase.h"
#include "Components/ActorComponent.h"
#include "Obstacles/Struct/CutterThrowParam.h"
#include "RotateTargetComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CUTTER_API URotateTargetComponent : public UTargetComponentBase
{
	GENERATED_BODY()

public:
	URotateTargetComponent();
	void RegisterParam(const FCutterThrowTargetParam& throwTargetParam);
	void Init();
	
private:
	virtual void Move(const FInputActionValue& Value) override;
	virtual void Throw(const FInputActionValue& Value) override;
	
private:
	FCutterThrowTargetParam _throwTargetParam;
};
