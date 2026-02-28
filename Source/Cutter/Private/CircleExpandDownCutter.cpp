// Fill out your copyright notice in the Description page of Project Settings.


#include "CircleExpandDownCutter.h"

// Sets default values for this component's properties
UCircleExpandDownCutter::UCircleExpandDownCutter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCircleExpandDownCutter::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

// Called every frame
void UCircleExpandDownCutter::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Translate(DeltaTime);
}

void UCircleExpandDownCutter::Init()
{
	_ownerActor = GetOwner();
	if (!_ownerActor)
	{
		UE_LOG(LogTemp, Error, TEXT("No ownerActor"));
		return;
	}
	FVector currentPos = _ownerActor->GetActorLocation();
	_rotateCenterPos = currentPos;
}

void UCircleExpandDownCutter::Translate(float DeltaTime)
{
	_ownerActor->SetActorLocation(CalcPosition(DeltaTime));
	_ownerActor->SetActorRotation(CalcRotation(DeltaTime));
}

FVector UCircleExpandDownCutter::CalcPosition(float DeltaTime)
{
	_currentAngle = _currentAngle + _moveRate * DeltaTime;
	float r = _pitch * _currentAngle;
	float sinValue, cosValue = 0.f;
	FMath::SinCos(&sinValue, &cosValue, _currentAngle);
	FVector moveVec = FVector(r * cosValue, r * sinValue, -_downSpeed * _currentAngle);
	FVector newVec = _rotateCenterPos + moveVec;
	return newVec;
}

FQuat UCircleExpandDownCutter::CalcRotation(float DeltaTime)
{
	FQuat rotation = FRotator(0, _rotateRate * DeltaTime * 100.f, 0).Quaternion();
	FQuat currentRotation = _ownerActor->GetActorRotation().Quaternion();

	return rotation * currentRotation;
}
