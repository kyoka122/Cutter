#include "StraightYoYoCutter.h"

#include "Cutter.h"
#include "Obstacles/CharacterTargetComponents/FullRotateTargetComponent.h"
#include "InGame/Interface/Damageable.h"
#include "InGame/Interface/ScoreTarget.h"
#include "InGame/Stage/StageShape.h"
#include "Struct/StraightYoYoThrowTargetParam.h"

void AStraightYoYoCutter::BeginPlay()
{
	Super::BeginPlay();
	_staticMeshComponent = GetStaticMesh();
	if (!IsValid(_staticMeshComponent))
	{
		UE_LOG(LogTemp, Log, TEXT("_staticMeshComponentが取得できませんでした。"));
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

FRotator AStraightYoYoCutter::CalcRotation(float deltaTime) const
{
	FRotator currentRotation = GetActorRotation();
	currentRotation.Yaw += _param.rotateRate * deltaTime * 100.f;
	return currentRotation;
}

void AStraightYoYoCutter::OnOverlapBreakableActor(AActor* otherActor)
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

void AStraightYoYoCutter::OnOverlapScoreTargetActor(AActor* otherActor) const
{
	if (otherActor && otherActor->GetClass()->ImplementsInterface(UScoreTarget::StaticClass()))
	{
		UE_LOG(LogCutter, Log, TEXT("AddScore %s by%s"), *GetName(), *otherActor->GetName());
		FScoreRobbedParam robbedParam = IScoreTarget::Execute_RobbedScore(otherActor, false);
		if (robbedParam.canRobScore)
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

void AStraightYoYoCutter::OnOverlapDamageableActor(AActor* otherActor) const
{
	if (otherActor && otherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		UE_LOG(LogCutter, Log, TEXT("AddDamage %s by%s"), *GetName(), *otherActor->GetName());
		IDamageable::Execute_Damage(otherActor, _param.damage, GetActorLocation());
	}
}

void AStraightYoYoCutter::Break()
{
	UE_LOG(LogCutter, Log, TEXT("Destroy %s"), *GetName());
	if (IsValid(this))
	{
		OnBreak();
	}
}

void AStraightYoYoCutter::StartTargeting_Implementation(AActor* throwActor)
{
	check(throwActor);
	UE_LOG(LogCutter, Log, TEXT("PrepareThrow %s by%s"), *GetName(), *throwActor->GetName());
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
	OnThrown();
	PlayMoveStartAnimation();
	SetActorHiddenInGame(false);
}

void AStraightYoYoCutter::SetThrowTargetParam()
{
	FVector2D currentPos = FVector2D(GetActorLocation());
	FVector stageCenterPos = IStageShape::Execute_GetCenterPos(_stageShape.GetObject());
	FVector2D moveVec = FVector2D(stageCenterPos) - currentPos;//TODO: この辺の値引数作って修正する
	
	FIntersectionData intersectionData = IStageShape::Execute_GetInterSections(_stageShape.GetObject(), currentPos, moveVec);
	_yoyoCenterPos = (intersectionData.point1 + intersectionData.point2) / 2;//中点
	_yoyoRadius2D = (intersectionData.point1 - _yoyoCenterPos);
	if (FMath::IsNearlyZero(_yoyoRadius2D.Y))
	{
		_offsetRad = FMath::Asin((currentPos.X - _yoyoCenterPos.X) / _yoyoRadius2D.X);
	}
	else
	{
		_offsetRad = FMath::Asin((currentPos.Y - _yoyoCenterPos.Y) / _yoyoRadius2D.Y);
	}
	
	//TODO: デバッグ用。実装終了後削除
	UE_LOG(LogCutter, Log, TEXT("stageCenterPos: %s"), *stageCenterPos.ToString());
	UE_LOG(LogTemp, Log, TEXT("intersectionData.point1: %s"), *intersectionData.point1.ToString());
	UE_LOG(LogTemp, Log, TEXT("intersectionData.point2: %s"), *intersectionData.point2.ToString());
	UE_LOG(LogTemp, Log, TEXT("_yoyoCenterPos: %s"), *_yoyoCenterPos.ToString());
	UE_LOG(LogTemp, Log, TEXT("_yoyoRadius2D: %s"), *_yoyoRadius2D.ToString());
	UE_LOG(LogTemp, Log, TEXT("_offsetRad: %f"), _offsetRad);
}

void AStraightYoYoCutter::OnBreak()
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