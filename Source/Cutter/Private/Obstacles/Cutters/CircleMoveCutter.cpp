#include "CircleMoveCutter.h"

#include "Cutter.h"
#include "Obstacles/CharacterTargetComponents/CircleMoveTargetComponent.h"
#include "InGame/Interface/Damageable.h"
#include "InGame/Interface/ScoreTarget.h"
#include "InGame/Stage/StageShape.h"
#include "Struct/CircleMoveCutterThrowTargetParam.h"

void ACircleMoveCutter::BeginPlay()
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

void ACircleMoveCutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Translate(DeltaTime);
}

void ACircleMoveCutter::Translate(float deltaTime)
{
	FTransform newTransform = FTransform(CalcRotation(deltaTime), CalcPosition(deltaTime));
	SetActorTransform(newTransform);
}

FVector ACircleMoveCutter::CalcPosition(float deltaTime)
{
	_currentRadius = FMath::Fmod(_currentRadius + _rotateDirection * _param.moveRate * deltaTime, 2 * UE_PI);
    if (_currentRadius < 0.0f)
    {
        _currentRadius += _rotateDirection * 2 * UE_PI;
    }

	float sinValue, cosValue = 0.f;
    FMath::SinCos(&sinValue, &cosValue, _currentRadius);
    FVector rotateVec = FVector(cosValue, sinValue, 0) * _rotateRadius;//半径と角度から回転後のベクトルを求める
    FVector newPosition = _rotateCenterPos + rotateVec;

   return newPosition;
}

FRotator ACircleMoveCutter::CalcRotation(float deltaTime) const
{
	FRotator currentRotation = GetActorRotation();
	currentRotation.Yaw += _param.rotateRate * deltaTime * 100.f;
    return currentRotation;
}

void ACircleMoveCutter::OnOverlapScoreTargetActor(AActor* otherActor) const
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

void ACircleMoveCutter::OnOverlapDamageableActor(AActor* otherActor) const
{
	if (otherActor && otherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		UE_LOG(LogCutter, Log, TEXT("AddDamage %s by%s"), *GetName(), *otherActor->GetName());
		IDamageable::Execute_Damage(otherActor, _param.damage, GetActorLocation());
	}
}

void ACircleMoveCutter::Break()
{
	UE_LOG(LogCutter, Log, TEXT("Break %s"), *GetName());
	if (IsValid(this))
	{
		OnBreak();
	}
}

void ACircleMoveCutter::StartTargeting_Implementation(AActor* throwActor)
{
	check(IsValid(throwActor))
	FVector2D currentPos2D = FVector2D(GetActorLocation());
	TObjectPtr<UCircleMoveTargetComponent> rotateTargetComponent = NewObject<UCircleMoveTargetComponent>(throwActor);
	
	FCircleMoveCutterThrowTargetParam throwTargetParam;
	throwTargetParam.rotateSpeed = _param.targetRotateSpeed;
	throwTargetParam.cutterPos = currentPos2D;
	throwTargetParam.accuracy = _param.targetAccuracy;
	throwTargetParam.segments = _param.segments;
	throwTargetParam.stageShape = _stageShape;
	throwTargetParam.looksTexture = _param.looksTexture;
	rotateTargetComponent->RegisterParam(throwTargetParam, [this](const FCircleMoveThrowParam& param){Throw(param);});
	
	rotateTargetComponent->RegisterComponent();
	rotateTargetComponent->Init();
}

void ACircleMoveCutter::Throw(const FCircleMoveThrowParam& param)
{
	UE_LOG(LogCutter, Log, TEXT("Throw %s"), *GetName());
	_rotateDirection = param.rotateDirection;
	ResetTransformParam(param.toStageCenterVec2D);
	SetActorTickEnabled(true);
	OnThrown();
	PlayMoveStartAnimation();
	SetActorHiddenInGame(false);
}

void ACircleMoveCutter::ResetTransformParam(FVector2D toStageCenterVec2D)
{
	FVector2D currentPos2D = FVector2D(GetActorLocation());
	_rotateRadius = toStageCenterVec2D.Size();
	FVector2D pointOfTangency = currentPos2D + 2 * toStageCenterVec2D;
	FVector2D rotateCenterPos2D = (pointOfTangency + currentPos2D) / 2;//円の端点2つ同士の中点から回転の中心点導出
	_rotateCenterPos = FVector(rotateCenterPos2D.X, rotateCenterPos2D.Y, IStageShape::Execute_GetCenterPos(_stageShape.GetObject()).Z);
	_currentRadius = FMath::Acos(FVector2D::DotProduct(FVector2D::UnitX(),-toStageCenterVec2D.GetSafeNormal()));//ベクトル同士の角度 = Acos(ベクトルの内積/各辺の大きさの積)
}

void ACircleMoveCutter::OnBreak()
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