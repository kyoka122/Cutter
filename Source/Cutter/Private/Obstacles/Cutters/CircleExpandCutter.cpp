#include "CircleExpandCutter.h"

#include "Cutter.h"
#include "InGame/Interface/Damageable.h"
#include "InGame/Interface/ScoreTarget.h"
#include "InGame/Stage/StageShape.h"
#include "Struct/CutterBaseParam.h"

void ACircleExpandCutter::BeginPlay()
{
	Super::BeginPlay();
	_staticMeshComponent = GetStaticMesh();
	if (!IsValid(_staticMeshComponent))
	{
		return;
	}
	InitTimeline(_staticMeshComponent);
	RegisterStaticMeshEvent(_staticMeshComponent, [this](AActor* otherActor)
	{
		OnOverlapBreakableActor(otherActor);
		OnOverlapScoreTargetActor(otherActor);
		OnOverlapDamageableActor(otherActor);
	});
}

void ACircleExpandCutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Translate(DeltaTime);
	CheckOutOfStage();
}

void ACircleExpandCutter::CheckOutOfStage()
{
	//MEMO: ステージ外に出たら寿命
	if (!IStageShape::Execute_IsInStage(_stageShape.GetObject(), FVector2D(GetActorLocation())))
	{
		OnBreak();
	}
}

void ACircleExpandCutter::ReStart()
{
	Super::ReStart();
	FVector currentPos = GetActorLocation();
	_rotateCenterPos = currentPos;
	SetActorTickEnabled(true);
	OnThrown();
	PlayMoveStartAnimation();
	SetActorHiddenInGame(false);
}

void ACircleExpandCutter::Translate(float deltaTime)
{
	SetActorLocation(CalcPosition(deltaTime));
	SetActorRotation(CalcRotation(deltaTime));
}

FVector ACircleExpandCutter::CalcPosition(float deltaTime)
{
	_currentAngle = _currentAngle + _param.moveRate * deltaTime;//角度加算
	float r = _param.pitch * _currentAngle;//動径算出 r=aθ
	float sinValue, cosValue = 0.f;
	FMath::SinCos(&sinValue, &cosValue, _currentAngle);
	
	//中心点に動径、角度によって決まった移動量x,yを加算する
	FVector newVec =  _rotateCenterPos + FVector(r * cosValue, r * sinValue, 0);
	return newVec;
}

FQuat ACircleExpandCutter::CalcRotation(float deltaTime) const
{
	FQuat rotation = FRotator(0, _param.rotateRate * deltaTime * 100.f, 0).Quaternion();
	FQuat currentRotation = GetActorRotation().Quaternion();

	return rotation * currentRotation;
}

void ACircleExpandCutter::OnOverlapBreakableActor(AActor* otherActor)
{
	if (this < otherActor)//MEMO: 同じタイプのオブジェクト同士の衝突=>衝突した際片方が判定するため
	{
		return;
	}
	if (IBreakable* otherBreakable = Cast<IBreakable>(otherActor))
	{
		UE_LOG(LogCutter, Log, TEXT("Break %s by%s"), *GetName(), *otherActor->GetName());
		otherBreakable->Break();
		OnBreak();
	}
}

void ACircleExpandCutter::OnOverlapScoreTargetActor(AActor* otherActor) const
{
	if (otherActor && otherActor->GetClass()->ImplementsInterface(UScoreTarget::StaticClass()))
	{
		UE_LOG(LogCutter, Log, TEXT("AddScore %s by%s"), *GetName(), *otherActor->GetName());
		FScoreRobbedParam robbedParam = IScoreTarget::Execute_RobbedScore(otherActor, false);
		if (!robbedParam.canRobScore)
		{
			return;
		}
		if (_scoreAddFunc)
		{
			_scoreAddFunc(robbedParam.score);
		}
		else UE_LOG(LogCutter, Error, TEXT("_scoreAddFunc 実行する関数がnullです %s"), *GetName());
	}
}

void ACircleExpandCutter::OnOverlapDamageableActor(AActor* otherActor) const
{
	if (otherActor && otherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		UE_LOG(LogCutter, Log, TEXT("AddDamage %s by%s"), *GetName(), *otherActor->GetName());
		IDamageable::Execute_Damage(otherActor, _param.damage, GetActorLocation());
	}
}

void ACircleExpandCutter::Break()
{
	UE_LOG(LogCutter, Log, TEXT("Break%s"), *GetName());
	if (IsValid(this))
	{
		OnBreak();
	}
}

void ACircleExpandCutter::OnBreak()
{
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	if (_releaseFunc)
	{
		_releaseFunc(this);
	}
	else UE_LOG(LogCutter, Error, TEXT("_releaseFunc 実行する関数がnullです %s"), *GetName());
}