#include "CircleExpandCutter.h"

UCircleExpandCutter::UCircleExpandCutter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UCircleExpandCutter::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

void UCircleExpandCutter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Translate(DeltaTime);
}

void UCircleExpandCutter::Init()
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

void UCircleExpandCutter::Translate(float DeltaTime)
{
	_ownerActor->SetActorLocation(CalcPosition(DeltaTime));
	_ownerActor->SetActorRotation(CalcRotation(DeltaTime));
}

FVector UCircleExpandCutter::CalcPosition(float DeltaTime)
{
	_currentAngle = _currentAngle + _moveRate * DeltaTime;//角度加算
	float r = _pitch * _currentAngle;//動径算出 r=aθ
	float sinValue, cosValue = 0.f;
	FMath::SinCos(&sinValue, &cosValue, _currentAngle);
	//中心点に動径、角度によって決まった移動量x,yを加算する
	FVector newVec =  _rotateCenterPos + FVector(r * cosValue, r * sinValue, 0);
	return newVec;
}

FQuat UCircleExpandCutter::CalcRotation(float DeltaTime)
{
	FQuat rotation = FRotator(0, _rotateRate * DeltaTime * 100.f, 0).Quaternion();
	FQuat currentRotation = _ownerActor->GetActorRotation().Quaternion();

	return rotation * currentRotation;
}
