#include "CircleMoveCutter.h"

#include "Cutter.h"
#include "Obstacles/CharacterTargetComponents/LimitedRotateTargetComponent.h"
#include "HAL/PreprocessorHelpers.h"
#include "InGame/Interface/Damageable.h"
#include "InGame/Interface/ScoreTarget.h"
#include "InGame/Stage/StageShape.h"
#include "Struct/CircleMoveCutterThrowTargetParam.h"

void ACircleMoveCutter::BeginPlay()
{
	Super::BeginPlay();
	_staticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	InitTimeline(_staticMeshComponent);
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
	FTransform newTransform = FTransform(CalcRotation(deltaTime), CalcPosition(deltaTime));
	SetActorTransform(newTransform);
}

FVector ACircleMoveCutter::CalcPosition(float deltaTime)
{
	_currentAngle = FMath::Fmod(_currentAngle + _param.moveRate * deltaTime, 2 * UE_PI);
    if (_currentAngle < 0.0f)
    {
        _currentAngle += 2 * UE_PI;
    }

	float sinValue, cosValue = 0.f;
    FMath::SinCos(&sinValue, &cosValue, _currentAngle);
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

void ACircleMoveCutter::OnOverlapBreakableActor(AActor* otherActor)
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

void ACircleMoveCutter::OnOverlapScoreTargetActor(AActor* otherActor) const
{
	
	if (otherActor && otherActor->GetClass()->ImplementsInterface(UScoreTarget::StaticClass()))
	{
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
	FVector2D pointOfTangency = IStageShape::Execute_GetFarPointOfTangency(_stageShape.GetObject(), FVector2D(currentPos2D));
	FVector2D toStageCenterVec2D = (pointOfTangency - currentPos2D)/2; //円の端点2つ同士の距離から半径ベクトル導出 
	ResetTransformParam(pointOfTangency, toStageCenterVec2D);
	
	TObjectPtr<ULimitedRotateTargetComponent> rotateTargetComponent = NewObject<ULimitedRotateTargetComponent>(throwActor);
	FCircleMoveCutterThrowTargetParam throwTargetParam;

	//TODO: 現状左向き確定なので、左用の値を入れる
	//throwTargetParam.firstLookVec = FVector2D(toStageCenterVec2D);
	throwTargetParam.firstLookVec = FVector2D(toStageCenterVec2D.Y, -toStageCenterVec2D.X);
	
	throwTargetParam.rightMaxVec = FVector2D(toStageCenterVec2D.Y, -toStageCenterVec2D.X);
	throwTargetParam.leftMaxVec = FVector2D(-toStageCenterVec2D.Y, toStageCenterVec2D.X);
	rotateTargetComponent->RegisterParam(throwTargetParam);
	rotateTargetComponent->RegisterThrowEvent(this);

	//TODO: 実装完了次第消す
	UE_LOG(LogTemp, Log, TEXT("pointOfTangency: %s"), *pointOfTangency.ToString());
	UE_LOG(LogTemp, Log, TEXT("throwActor: %s, %p"), *throwActor->GetName(), throwActor);
	UE_LOG(LogTemp, Log, TEXT("throwTargetParam.firstLookVec : %s"), *throwTargetParam.firstLookVec.ToString());
	UE_LOG(LogTemp, Log, TEXT("throwTargetParam.rightMaxVec : %s"), *throwTargetParam.rightMaxVec.ToString());
	UE_LOG(LogTemp, Log, TEXT("throwTargetParam.leftLookVec : %s"), *throwTargetParam.leftMaxVec.ToString());
	
	rotateTargetComponent->RegisterComponent();
	rotateTargetComponent->Init();
}

void ACircleMoveCutter::Throw_Implementation()//TODO: 引数追加
{
	//TODO: 引数追加
	UE_LOG(LogCutter, Log, TEXT("Throw %s"), *GetName());
	SetActorTickEnabled(true);
	OnThrown();
	PlayMoveStartAnimation();
	SetActorHiddenInGame(false);
}

void ACircleMoveCutter::ResetTransformParam(FVector2D pointOfTangency, FVector2D toStageCenterVec2D)
{
	FVector2D currentPos2D = FVector2D(GetActorLocation());
	_rotateRadius = toStageCenterVec2D.Size();
	FVector2D rotateCenterPos2D = (pointOfTangency + currentPos2D) / 2;//円の端点2つ同士の中点から回転の中心点導出
	_rotateCenterPos = FVector(rotateCenterPos2D.X, rotateCenterPos2D.Y, IStageShape::Execute_GetCenterPos(_stageShape.GetObject()).Z);
	toStageCenterVec2D.Normalize();
	_currentAngle = FMath::Acos(FVector2D::DotProduct(FVector2D::UnitX(),-toStageCenterVec2D));//ベクトル同士の角度 = Acos(ベクトルの内積/各辺の大きさの積(Normalize済)なのでなし)
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