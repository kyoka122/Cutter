// Fill out your copyright notice in the Description page of Project Settings.


#include "CircleMoveCutter.h"

#include "HAL/PreprocessorHelpers.h"

// Sets default values for this component's properties
UCircleMoveCutter::UCircleMoveCutter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCircleMoveCutter::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

// Called every frame
void UCircleMoveCutter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Translate(DeltaTime);
}

void UCircleMoveCutter::Init()
{
	_ownerActor = GetOwner();
	if (!_ownerActor)
	{
		UE_LOG(LogTemp, Error, TEXT("No ownerActor"));
		return;
	}

	FVector currentPos = _ownerActor->GetActorLocation();
	float radius = _stageSize / 2.f - FMath::Abs(currentPos.X / 2);
	FVector toStageCenterVec = _stageCenterPos - currentPos;
	FVector toStageCenterVec2D = FVector(toStageCenterVec.X, toStageCenterVec.Y, 0);
	_rotateRadius = (toStageCenterVec2D.Size() + radius) / 2;
	toStageCenterVec2D.Normalize();
	_rotateCenterPos = currentPos + toStageCenterVec2D * _rotateRadius;
}

void UCircleMoveCutter::Translate(float DeltaTime)
{
	_ownerActor->SetActorLocation(CalcPosition(DeltaTime));
	_ownerActor->SetActorRotation(CalcRotation(DeltaTime));
}

FVector UCircleMoveCutter::CalcPosition(float DeltaTime)
{
	_currentAngle = FMath::Fmod(_currentAngle + _moveRate * DeltaTime, 360.0f);
    if (_currentAngle < 0.0f)
    {
        _currentAngle += 360.0f;
    }

	float sinValue, cosValue = 0.f;
    FMath::SinCos(&sinValue, &cosValue, _currentAngle);
    FVector rotateVec = FVector(cosValue, sinValue, 0) * _rotateRadius;//半径と角度から回転後のベクトルを求める
    FVector newPosition = _rotateCenterPos + rotateVec;

	//スケール x 回転 x 移動
	//Matrix4x4 matrix = Matrix4x4.TRS(translate, rotation, FVector.one);
	//point = matrix.MultiplyPoint3x4(point);

   return newPosition;
}

FQuat UCircleMoveCutter::CalcRotation(float DeltaTime)
{
	FQuat rotation = FRotator(0, _rotateRate * DeltaTime * 100.f, 0).Quaternion();
	FQuat currentRotation = _ownerActor->GetActorRotation().Quaternion();

    return rotation * currentRotation;
}
