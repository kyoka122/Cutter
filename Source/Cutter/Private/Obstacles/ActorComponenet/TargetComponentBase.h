// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetComponentBase.generated.h"


class IThrowable;
struct FInputActionValue;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CUTTER_API UTargetComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UTargetComponentBase(){}
	void RegisterThrowable(AActor* throwable);
	
protected:
	void CacheArrowMesh();
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
	TObjectPtr<AActor> _owner;
	
private:
	TObjectPtr<UStaticMeshComponent> _throwArrowMesh;
	int32 _bindingHandle;
};
