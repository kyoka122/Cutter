#include "CircleMoveCutter.h"
#include "HAL/PreprocessorHelpers.h"
#include "InGame/Interface/Damageable.h"
#include "InGame/Interface/ScoreTarget.h"
#include "InGame/ObstacleSpawner.h"

void ACircleMoveCutter::BeginPlay()
{
	Super::BeginPlay();
	_staticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	RegisterStaticMeshEvent(_staticMeshComponent, [this](AActor* otherActor)
	{
		OnOverlapBreakableActor(otherActor);
		OnOverlapScoreTargetActor(otherActor);
		OnOverlapDamageableActor(otherActor);
	});
}

void ACircleMoveCutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Translate(DeltaTime);
}

void ACircleMoveCutter::Translate(float deltaTime)
{
	SetActorLocation(CalcPosition(deltaTime));
	SetActorRotation(CalcRotation(deltaTime));
}

FVector ACircleMoveCutter::CalcPosition(float deltaTime)
{
	_currentAngle = FMath::Fmod(_currentAngle + _param.moveRate * deltaTime, 360.0f);
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
	FQuat rotation = FRotator(0, _param.rotateRate * deltaTime * 100.f, 0).Quaternion();
	FQuat currentRotation = GetActorRotation().Quaternion();

    return rotation * currentRotation;
}

void ACircleMoveCutter::OnOverlapBreakableActor(AActor* otherActor)
{
	if (this < otherActor)//MEMO:衝突した際片方が判定するため
	{
		return;
	}
	if (IBreakable* otherBreakable = Cast<IBreakable>(otherActor))
	{
		SetActorEnableCollision(false);
		otherActor->SetActorEnableCollision(false);
		UE_LOG(LogTemp, Log, TEXT("Destroy01,%s"), *otherActor->GetName());
		otherBreakable->Break();
		OnBreak();
	}
}

void ACircleMoveCutter::OnOverlapScoreTargetActor(AActor* otherActor)
{
	if (IScoreTarget* otherScoreTarget = Cast<IScoreTarget>(otherActor))
	{
		FScoreRobbedParam robbedParam = otherScoreTarget->RobbedScore_Implementation(false);
		if (robbedParam.canRobScore)
		{
			return;
		}
		if (!_scoreAddFunc)
		{
			UE_LOG(LogTemp, Error, TEXT("_scoreAddFunc 実行する関数がnullです"));
			return;
		}
		_scoreAddFunc(robbedParam.score);
	}
}

void ACircleMoveCutter::OnOverlapDamageableActor(AActor* otherActor)
{
	if (otherActor && otherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("AddDamage"));
		IDamageable::Execute_Damage(otherActor, _param.Damage, GetActorLocation());
	}
}

void ACircleMoveCutter::Break()
{
	UE_LOG(LogTemp, Log, TEXT("Imp_Destroy01"));
	if (IsValid(this))
	{
		OnBreak();
	}
}

void ACircleMoveCutter::StartTargeting_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("PrepareThrow"));
}

void ACircleMoveCutter::Throw_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Throw 01"));
	ResetTransformParam();
	StartTick();
	SetActorHiddenInGame(false);
	LazyActiveStaticMeshEvent();
}

void ACircleMoveCutter::LazyActiveStaticMeshEvent()
{
	GetWorldTimerManager().SetTimer(
		_overlapActiveTimerHandle,
		[this]{SetActorEnableCollision(true);},
		1.0f,
		false
	);
	UE_LOG(LogTemp, Log, TEXT("LazyRegisterStaticMeshEvent"));
}

void ACircleMoveCutter::ResetTransformParam()
{
	FVector currentPos = GetActorLocation();
	float radius = _stageEnvironmentParam->stageSize / 2.f - FMath::Abs(currentPos.X / 2);
	FVector toStageCenterVec = _stageEnvironmentParam->centerPos - currentPos;
	FVector toStageCenterVec2D = FVector(toStageCenterVec.X, toStageCenterVec.Y, 0);
	_rotateRadius = (toStageCenterVec2D.Size() + radius) / 2;
	toStageCenterVec2D.Normalize();
	_rotateCenterPos = currentPos + toStageCenterVec2D * _rotateRadius;
}

void ACircleMoveCutter::OnBreak()
{
	SetActorEnableCollision(false);
	StopTick();
	SetActorHiddenInGame(true);
	if (!_inactiveFunc)
	{
		UE_LOG(LogTemp, Error, TEXT("_inactiveFunc 実行する関数がnullです"));
		return;
	}
	_inactiveFunc();
}