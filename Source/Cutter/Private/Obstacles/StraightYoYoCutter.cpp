#include "StraightYoYoCutter.h"

#include "Cutter.h"
#include "ActorComponenet/FullRotateTargetComponent.h"
#include "InGame/Interface/Damageable.h"
#include "InGame/Interface/ScoreTarget.h"
#include "InGame/Stage/StageShape.h"
#include "Struct/StraightYoYoThrowTargetParam.h"
#include "MathUtil.h"

void AStraightYoYoCutter::BeginPlay()
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

void AStraightYoYoCutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Translate(DeltaTime);
}

void AStraightYoYoCutter::Translate(float deltaTime)
{
	FTransform newTransform = FTransform(CalcRotation(deltaTime), CalcPosition(deltaTime));
	SetActorTransform(newTransform);
}

FVector AStraightYoYoCutter::CalcPosition(float deltaTime)
{
	_currentTime = FMath::Fmod(_currentTime + deltaTime, 2 * UE_PI);
	if (_currentTime < 0.0f)
	{
		_currentTime += 2 * UE_PI;
	}
	FVector2D newPos = _yoyoRadius2D * FMath::Sin(_param.radianFrequency * _currentTime + _offsetRad) + _yoyoCenterPos;
	FVector stageCenterPos = IStageShape::Execute_GetCenterPos(_stageShape.GetObject());
	return FVector(newPos.X, newPos.Y, stageCenterPos.Z);
}

FRotator AStraightYoYoCutter::CalcRotation(float deltaTime)
{
	FRotator currentRotation = GetActorRotation();
	currentRotation.Yaw += _param.rotateRate * deltaTime * 100.f;
	return currentRotation;
}

void AStraightYoYoCutter::OnOverlapBreakableActor(AActor* otherActor)
{
	if (IBreakable* otherBreakable = Cast<IBreakable>(otherActor))
	{
		UE_LOG(LogCutter, Log, TEXT("Break %s by%s"), *GetName(), *otherActor->GetName());
		otherBreakable->Break();
		OnBreak();
	}
}

void AStraightYoYoCutter::OnOverlapScoreTargetActor(AActor* otherActor)
{
	if (IScoreTarget* otherScoreTarget = Cast<IScoreTarget>(otherActor))
	{
		UE_LOG(LogCutter, Log, TEXT("AddScore %s by%s"), *GetName(), *otherActor->GetName());
		FScoreRobbedParam robbedParam = otherScoreTarget->RobbedScore_Implementation(false);
		if (robbedParam.canRobScore)
		{
			return;
		}
		if (!_scoreAddFunc)
		{
			UE_LOG(LogCutter, Error, TEXT("_scoreAddFunc 実行する関数がnullです %s"), *GetName());
			return;
		}
		_scoreAddFunc(robbedParam.score);
	}
}

void AStraightYoYoCutter::OnOverlapDamageableActor(AActor* otherActor)
{
	if (otherActor && otherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		UE_LOG(LogCutter, Log, TEXT("AddDamage %s by%s"), *GetName(), *otherActor->GetName());
		IDamageable::Execute_Damage(otherActor, _param.Damage, GetActorLocation());
	}
}

void AStraightYoYoCutter::Break()
{
	UE_LOG(LogCutter, Log, TEXT("Imp_Destroy %s"), *GetName());
	if (IsValid(this))
	{
		OnBreak();
	}
}

void AStraightYoYoCutter::StartTargeting_Implementation(AActor* throwActor)
{
	check(throwActor);
	UE_LOG(LogCutter, Log, TEXT("PrepareThrow %s"), *GetName());
	FVector currentPos = GetActorLocation();
	
	UFullRotateTargetComponent* fullRotateTargetComponent = NewObject<UFullRotateTargetComponent>(throwActor);
	FStraightYoYoThrowTargetParam straightYoYoThrowTargetParam;
	FVector stageCenterPos = IStageShape::Execute_GetCenterPos(_stageShape.GetObject());
	straightYoYoThrowTargetParam.firstLookVec = FVector2D(stageCenterPos - currentPos);
	
	fullRotateTargetComponent->RegisterParam(straightYoYoThrowTargetParam);
	fullRotateTargetComponent->RegisterThrowEvent(this);
	fullRotateTargetComponent->RegisterComponent();
	fullRotateTargetComponent->Init();
}

void AStraightYoYoCutter::Throw_Implementation()
{
	UE_LOG(LogCutter, Log, TEXT("Throw %s"), *GetName());
	SetThrowTargetParam();
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	LazyActiveStaticMeshEvent();
}

void AStraightYoYoCutter::LazyActiveStaticMeshEvent()
{
	GetWorldTimerManager().SetTimer(
		_overlapActiveTimerHandle,
		[this]{SetActorEnableCollision(true);},
		1.0f,
		false
	);
}

void AStraightYoYoCutter::SetThrowTargetParam()
{
	FVector2D currentPos = FVector2D(GetActorLocation());
	FVector stageCenterPos = IStageShape::Execute_GetCenterPos(_stageShape.GetObject());
	FVector2D moveVec = FVector2D(currentPos - FVector2D(stageCenterPos));//TODO: この辺の値引数作って修正する
	
	//FIntersectionData intersectionData = IStageShape::Execute_GetInterSection(_stageShape.GetObject(), currentPos, moveVec);
	// _yoyoCenterPos = (intersectionData.point1 + intersectionData.point2) / 2;
	// _yoyorRadius2D = (intersectionData.point1 - _yoyoCenterPos);
	//↑に戻す
	_yoyoCenterPos = FVector2D::Zero();//仮置き
	_yoyoRadius2D = FVector2D(300, 300);//借り置き
	_offsetRad = FMath::Asin((currentPos.X - _yoyoCenterPos.X) / _yoyoRadius2D.X);

	UE_LOG(LogTemp, Log, TEXT("_yoyoCenterPos: %s"), *_yoyoCenterPos.ToString());
	UE_LOG(LogTemp, Log, TEXT("_yoyoRadius2D: %s"), *_yoyoRadius2D.ToString());
	UE_LOG(LogTemp, Log, TEXT("_offsetRad: %f"), _offsetRad);
}

void AStraightYoYoCutter::OnBreak()
{
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	if (!_inactiveFunc)
	{
		UE_LOG(LogCutter, Error, TEXT("_inactiveFunc 実行する関数がnullです %s"), *GetName());
		return;
	}
	_inactiveFunc();
}