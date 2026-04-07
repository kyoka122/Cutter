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
	void VisibleArrowMesh();
	void InVisibleArrowMesh();
	void RegisterInputComponent();
	void ReleaseInputComponent();
	virtual void Move(const FInputActionValue& Value){}
	virtual void Throw(const FInputActionValue& Value){}
	
protected:
	UPROPERTY()
	TScriptInterface<IThrowable> _throwable;
	TObjectPtr<AController> _controller;
	TObjectPtr<ACutterCharacter> _owner;
	TObjectPtr<UCameraComponent> _followCamera;
	TObjectPtr<UStaticMeshComponent> _throwArrowMesh;
	
private:
	int32 _bindingHandle;
};
