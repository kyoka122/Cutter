// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CircleExpandCutter.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CUTTER_API UCircleExpandCutter : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCircleExpandCutter();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void Init();

private:
	void Translate(float DeltaTime);
	FVector CalcPosition(float DeltaTime);
	FQuat CalcRotation(float DeltaTime);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "回転パラメータ", meta = (AllowPrivateAccess = "true"))
	float _rotateRate = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "移動パラメータ", meta = (AllowPrivateAccess = "true"))
	float _moveRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "移動パラメータ", meta = (AllowPrivateAccess = "true"))
	FVector _rotateCenterPos = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "移動パラメータ", meta = (AllowPrivateAccess = "true"))
	float _pitch = 10.f;

	AActor* _ownerActor = nullptr;
	float _currentAngle = 0.0f;
};
