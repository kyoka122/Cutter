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
	void RegisterThrowEvent(AActor* throwable);
	
protected:
	virtual void Init();
	void CacheArrowMesh();
	void CacheOwner();
	void CacheCamera();
	void VisibleArrowMesh() const;
	void InVisibleArrowMesh() const;
	void RegisterInputComponent();
	void ReleaseInputComponent() const;
	virtual void Move(const FInputActionValue& Value){}
	virtual void Throw(const FInputActionValue& Value){}
	
protected:
	UPROPERTY() TScriptInterface<IThrowable> _throwable;
	UPROPERTY() TObjectPtr<AController> _controller;
	UPROPERTY() TObjectPtr<ACutterCharacter> _owner;
	UPROPERTY() TObjectPtr<UCameraComponent> _followCamera;
	UPROPERTY() TObjectPtr<UStaticMeshComponent> _throwArrowMesh;
	
private:
	int32 _bindingHandle;
};
