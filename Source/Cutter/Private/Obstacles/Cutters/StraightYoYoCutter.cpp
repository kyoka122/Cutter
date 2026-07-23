#include "StraightYoYoCutter.h"

#include "Cutter.h"
#include "Obstacles/CharacterTargetComponents/StraightYoYoTargetComponent.h"
#include "InGame/Interface/Damageable.h"
#include "InGame/Interface/ScoreTarget.h"
#include "InGame/Stage/StageShape.h"
#include "Obstacles/CharacterTargetComponents/Struct/StraightYoYoThrowParam.h"
#include "Struct/StraightYoYoThrowTargetParam.h"

void AStraightYoYoCutter::BeginPlay()
{
	Super::BeginPlay();
	_staticMeshComponent = GetStaticMesh();
	if (!IsValid(_staticMeshComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("_staticMeshComponentが取得できませんでした。"));
		return;
	}
	InitTimeline(_staticMeshComponent);
	RegisterStaticMeshEvent(_staticMeshComponent, [this](AActor* otherActor)
	{
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
	//x = A * sin(Tt + α)
	//位相 = 半径 * sin(角振動数 * 時間 + 初期)
	FVector2D newPos = _yoyoRadius2D * FMath::Sin(_param.radianFrequency * _currentTime + _offsetRad) + _yoyoCenterPos;
	FVector stageCenterPos = FVector::ZeroVector;
	if (_stageShape)
	{
		stageCenterPos = IStageShape::Execute_GetCenterPos(_stageShape.GetObject());
	}
	return FVector(newPos.X, newPos.Y, stageCenterPos.Z);
}

FRotator AStraightYoYoCutter::CalcRotation(float deltaTime) const
{
	FRotator currentRotation = GetActorRotation();
	currentRotation.Yaw += _param.rotateSpeed * deltaTime * 100.f;
	return currentRotation;
}

void AStraightYoYoCutter::OnOverlapScoreTargetActor(AActor* otherActor) const
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
	
	UStraightYoYoTargetComponent* straightYoYoTargetComponent = NewObject<UStraightYoYoTargetComponent>(throwActor);
	FStraightYoYoThrowTargetParam straightYoYoThrowTargetParam;
	FVector stageCenterPos = IStageShape::Execute_GetCenterPos(_stageShape.GetObject());
	
	straightYoYoThrowTargetParam.firstLookVec = FVector2D(stageCenterPos - currentPos);//MEMO: 初回は中央を向かせる
	straightYoYoThrowTargetParam.rotateSpeed = _param.targetRotateSpeed;
	straightYoYoThrowTargetParam.cutterPos = FVector2D(currentPos);
	straightYoYoThrowTargetParam.stageShape = _stageShape;
	straightYoYoThrowTargetParam.looksTexture = _param.looksTexture;
	
	straightYoYoTargetComponent->RegisterParam(straightYoYoThrowTargetParam, [this](const FStraightYoYoThrowParam& param){Throw(param);});
	straightYoYoTargetComponent->RegisterComponent();
	straightYoYoTargetComponent->Init();
}

void AStraightYoYoCutter::Throw(const FStraightYoYoThrowParam& param)
{
	UE_LOG(LogCutter, Log, TEXT("Throw %s"), *GetName());
	SetThrowTargetParam(param);
	SetActorTickEnabled(true);
	OnThrown();
	PlayMoveStartAnimation();
	SetActorHiddenInGame(false);
}

void AStraightYoYoCutter::SetThrowTargetParam(const FStraightYoYoThrowParam& param)
{
	FVector2D currentPos = FVector2D(GetActorLocation());
	
	FIntersectionData intersectionData = IStageShape::Execute_GetInterSections(_stageShape.GetObject(), currentPos, FVector2D(param.moveVec));
	_yoyoCenterPos = (intersectionData.point1 + intersectionData.point2) / 2;//中点
	_yoyoRadius2D = intersectionData.point1 - _yoyoCenterPos;
	
	//MEMO: X軸、Y軸に平行な場合、同じ式ではoffsetを割り出せないので、条件分岐を行う。
	//X軸に平行な時はXの差分からoffsetを割り出せる。Y軸と、それ以外の条件の場合はどちらでもいいので、Y軸差分から割り出す。
	if (FMath::IsNearlyZero(_yoyoRadius2D.Y))
	{
		_offsetRad = FMath::Asin((currentPos.X - _yoyoCenterPos.X) / _yoyoRadius2D.X);
	}
	else
	{
		_offsetRad = FMath::Asin((currentPos.Y - _yoyoCenterPos.Y) / _yoyoRadius2D.Y);
	}
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