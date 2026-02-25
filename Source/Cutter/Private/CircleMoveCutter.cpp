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
    ownerActor = GetOwner();
    if (ownerActor) {
		FVector currentPos = ownerActor -> GetActorLocation();
    	UE_LOG(LogTemp, Log, TEXT("Init"));
        Init(currentPos, FVector::ZeroVector, 13 / 2.f - currentPos.X / 2);
    }
}

// Called every frame
void UCircleMoveCutter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    Move(DeltaTime);
}

void UCircleMoveCutter::Init(FVector currentPos, FVector stageCenterPos, float radius)
{
    FVector toStageCenterVec = stageCenterPos - currentPos;
    FVector toStageCenterVec2 = FVector(toStageCenterVec.X, 0, toStageCenterVec.Z);
    _rotateRadius = (toStageCenterVec2.Size() + radius) / 2;
    _rotateCenterPos = currentPos + toStageCenterVec2.Normalize() * _rotateRadius;
    //_currentAngle = Mathf.Atan2(-toStageCenterVec2.z, -toStageCenterVec2.x);
}

void UCircleMoveCutter::Move(float DeltaTime)
{
    ownerActor->SetActorLocation(CalcPosition(DeltaTime));
    ownerActor->SetActorRotation(CalcRotation(DeltaTime));
}

FVector UCircleMoveCutter::CalcPosition(float DeltaTime)
{
    _currentAngle = FMath::Fmod(_currentAngle + moveRate * DeltaTime, 360.0f);
    if (_currentAngle < 0.0f)
    {
        _currentAngle += 360.0f;
    }
	
    float sinValue, cosValue;
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
    FQuat rotation = FRotator(0,  rotateRate * DeltaTime, 0).Quaternion();
    FQuat currentRotation = ownerActor -> GetActorRotation().Quaternion();

    return rotation * currentRotation;
}
