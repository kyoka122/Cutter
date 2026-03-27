#include "CircleExpandCutter.h"
#include "InGame/Interface/Damageable.h"
#include "InGame/Interface/ScoreTarget.h"
#include "Struct/CutterBaseParam.h"

void ACircleExpandCutter::BeginPlay()
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

void ACircleExpandCutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Translate(DeltaTime);
}

void ACircleExpandCutter::ReStart()
{
	Super::ReStart();
	FVector currentPos = GetActorLocation();
	_param.rotateCenterPos = currentPos;
	StartTick();
	SetActorHiddenInGame(false);
	LazyActiveStaticMeshEvent();
}

void ACircleExpandCutter::LazyActiveStaticMeshEvent()
{
	GetWorldTimerManager().SetTimer(
		_overlapActiveTimerHandle,
		[this]{SetActorEnableCollision(true);},
		1.0f,
		false
	);
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
	FVector newVec =  _param.rotateCenterPos + FVector(r * cosValue, r * sinValue, 0);
	return newVec;
}

FQuat ACircleExpandCutter::CalcRotation(float deltaTime)
{
	FQuat rotation = FRotator(0, _param.rotateRate * deltaTime * 100.f, 0).Quaternion();
	FQuat currentRotation = GetActorRotation().Quaternion();

	return rotation * currentRotation;
}

void ACircleExpandCutter::OnOverlapBreakableActor(AActor* otherActor)
{
	if (this < otherActor)//同じタイプのオブジェクト同士の衝突=>衝突した際片方が判定するため
	{
		return;
	}
	if (IBreakable* otherBreakable = Cast<IBreakable>(otherActor))
	{
		SetActorEnableCollision(false);
		otherActor->SetActorEnableCollision(false);
		UE_LOG(LogTemp, Log, TEXT("Destroy02,%s"), *otherActor->GetName());
		otherBreakable->Break();
		OnBreak();
	}
}

void ACircleExpandCutter::OnOverlapScoreTargetActor(AActor* otherActor)
{
	if (IScoreTarget* otherScoreTarget = Cast<IScoreTarget>(otherActor))
	{
		UE_LOG(LogTemp, Log, TEXT("AddScore"));
		FScoreRobbedParam robbedParam = otherScoreTarget->RobbedScore_Implementation(false);
		if (robbedParam.canRobScore)
		{
			if (!_scoreAddFunc)
			{
				UE_LOG(LogTemp, Error, TEXT("_scoreAddFunc 実行する関数がnullです"));
				return;
			}
			_scoreAddFunc(robbedParam.score);
		}
	}
}

void ACircleExpandCutter::OnOverlapDamageableActor(AActor* otherActor)
{
	if (IDamageable* otherDamageable = Cast<IDamageable>(otherActor))
	{
		UE_LOG(LogTemp, Log, TEXT("AddDamage"));
		otherDamageable->Damage(_param.Damage, GetActorLocation());
		//演出実行
	}
}

void ACircleExpandCutter::Break()
{
	UE_LOG(LogTemp, Log, TEXT("Imp_Destroy02"));
	if (IsValid(this))
	{
		OnBreak();
	}
}

void ACircleExpandCutter::OnBreak()
{
	SetActorEnableCollision(false);
	StopTick();
	SetActorHiddenInGame(true);
	if (!_deactiveFunc)
	{
		UE_LOG(LogTemp, Error, TEXT("_deactiveFunc 実行する関数がnullです"));
		return;
	}
	_deactiveFunc();
}