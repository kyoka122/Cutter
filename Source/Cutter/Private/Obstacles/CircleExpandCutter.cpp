#include "CircleExpandCutter.h"

void ACircleExpandCutter::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void ACircleExpandCutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Translate(DeltaTime);
}

void ACircleExpandCutter::Init()
{
	FVector currentPos = GetActorLocation();
	param.rotateCenterPos = currentPos;
}

void ACircleExpandCutter::Translate(float deltaTime)
{
	SetActorLocation(CalcPosition(deltaTime));
	SetActorRotation(CalcRotation(deltaTime));
}

FVector ACircleExpandCutter::CalcPosition(float deltaTime)
{
	_currentAngle = _currentAngle + param.moveRate * deltaTime;//角度加算
	float r = param.pitch * _currentAngle;//動径算出 r=aθ
	float sinValue, cosValue = 0.f;
	FMath::SinCos(&sinValue, &cosValue, _currentAngle);
	//中心点に動径、角度によって決まった移動量x,yを加算する
	FVector newVec =  param.rotateCenterPos + FVector(r * cosValue, r * sinValue, 0);
	return newVec;
}

FQuat ACircleExpandCutter::CalcRotation(float deltaTime)
{
	FQuat rotation = FRotator(0, param.rotateRate * deltaTime * 100.f, 0).Quaternion();
	FQuat currentRotation = GetActorRotation().Quaternion();

	return rotation * currentRotation;
}
