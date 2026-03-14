#include "CircleMoveCutter.h"
#include "HAL/PreprocessorHelpers.h"

void ACircleMoveCutter::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void ACircleMoveCutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Translate(DeltaTime);
}

void ACircleMoveCutter::Init()
{
	FVector currentPos = GetActorLocation();
	float radius = param.stageSize / 2.f - FMath::Abs(currentPos.X / 2);
	FVector toStageCenterVec = param.stageCenterPos - currentPos;
	FVector toStageCenterVec2D = FVector(toStageCenterVec.X, toStageCenterVec.Y, 0);
	_rotateRadius = (toStageCenterVec2D.Size() + radius) / 2;
	toStageCenterVec2D.Normalize();
	_rotateCenterPos = currentPos + toStageCenterVec2D * _rotateRadius;
}

void ACircleMoveCutter::Translate(float deltaTime)
{
	SetActorLocation(CalcPosition(deltaTime));
	SetActorRotation(CalcRotation(deltaTime));
}

FVector ACircleMoveCutter::CalcPosition(float deltaTime)
{
	_currentAngle = FMath::Fmod(_currentAngle + param.moveRate * deltaTime, 360.0f);
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

FQuat ACircleMoveCutter::CalcRotation(float deltaTime)
{
	FQuat rotation = FRotator(0, param.rotateRate * deltaTime * 100.f, 0).Quaternion();
	FQuat currentRotation = GetActorRotation().Quaternion();

    return rotation * currentRotation;
}
