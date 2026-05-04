#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetComponentBase.generated.h"

class UCameraComponent;
class ACutterCharacter;
class IThrowable;
struct FInputActionValue;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CUTTER_API UTargetComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UTargetComponentBase(){}
	
protected:
	virtual void Init();
	void CacheOwner();
	void CacheOverViewCapture();
	void RegisterInputComponent();
	void ReleaseInputComponent() const;
	virtual void Rotate(const FInputActionValue& Value){}
	virtual void Throw(const FInputActionValue& Value){}
	
protected:
	UPROPERTY() TObjectPtr<AController> _controller;
	UPROPERTY() TObjectPtr<ACutterCharacter> _owner;
	UPROPERTY() TObjectPtr<USceneCaptureComponent2D> _overViewCapture;
	UPROPERTY() TObjectPtr<UStaticMeshComponent> _throwArrowMesh;
	
private:
	int32 _bindingHandle;
};
