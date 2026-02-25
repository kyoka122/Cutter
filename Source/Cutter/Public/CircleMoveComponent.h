// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CircleMoveComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CUTTER_API UCircleMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCircleMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void Init(FVector currentPos, FVector stageCenterPos, float radius);
	void Move(float DeltaTime);
	FVector CalcPosition(float DeltaTime);
	FQuat CalcRotation(float DeltaTime);


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "test", meta = (AllowPrivateAccess = "true"))
	float moveRate = 0.001f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "test2", meta = (AllowPrivateAccess = "true"))
	float rotateRate = 0.01f;

	AActor* ownerActor;
	FVector _rotateCenterPos;
	float _rotateRadius;
	float _currentAngle;
};