// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CircleMoveCutter.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUTTER_API UCircleMoveCutter : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCircleMoveCutter();

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float moveRate = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float rotateRate = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector _rotateCenterPos = FVector(0.0f, 0.0f, 0.0f);
	
	AActor* ownerActor = nullptr;
	float _rotateRadius = 0.0f;
	float _currentAngle = 0.0f;
};