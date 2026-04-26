#include "FractalCircleMoveCutter.h"

#include "Cutter.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Obstacles/CharacterTargetComponents/CircleMoveTargetComponent.h"
#include "InGame/Interface/Damageable.h"
#include "InGame/Interface/ScoreTarget.h"
#include "InGame/Stage/StageShape.h"
#include "Struct/CircleMoveCutterThrowTargetParam.h"

void AFractalCircleMoveCutter::BeginPlay()
{
	Super::BeginPlay();
	_ismComponent = GetIsmStaticMesh();
	if (!IsValid(_ismComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("_instancedStaticMeshComponentが取得できませんでした。"));
		return;
	}
	InitTimeline(_ismComponent);
	RegisterStaticMeshEvent(_ismComponent, [this](AActor* otherActor)
	{
		//OnOverlapBreakableActor(otherActor);
		OnOverlapScoreTargetActor(otherActor);
		OnOverlapDamageableActor(otherActor);
	});
	InstanceCutterChildren();
}

void AFractalCircleMoveCutter::InstanceCutterChildren()
{
	float k =static_cast<float>(_param.childCountPerLayer);
	_totalInstanceCount = (FMath::Pow(k, _param.depth) - 1) / (k - 1);//MEMO: 等比数列
	for (int i = 0; i < _totalInstanceCount; i++)
	{
		_ismComponent->AddInstance(FTransform::Identity);
	}
}

void AFractalCircleMoveCutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Translate(DeltaTime);
}

void AFractalCircleMoveCutter::Translate(float deltaTime)
{
	FTransform newTransform = FTransform(CalcParentRotation(deltaTime), CalcParentPosition(deltaTime));
	SetActorTransform(newTransform);
	
	FVector baseScale =_ismComponent->GetRelativeScale3D();
	FScaleMatrix baseScaleMatrix(baseScale);
	FMatrix parentTransformMatrix = baseScaleMatrix * newTransform.ToMatrixWithScale();
	
	int index = 1;
	_ismComponent->UpdateInstanceTransform(0, FTransform(parentTransformMatrix), true, false, true);
	TransformCutterChildren(parentTransformMatrix, _param.depth - 1, index);
	
	_ismComponent->RecreatePhysicsState();
	_ismComponent->MarkRenderStateDirty();//MEMO: ↑で更新が終わるので、ここでまとめて描画更新対象にする
}

FVector AFractalCircleMoveCutter::CalcParentPosition(float deltaTime)
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

FRotator AFractalCircleMoveCutter::CalcParentRotation(float deltaTime) const
{
	FRotator currentRotation = GetActorRotation();
	currentRotation.Yaw += _param.rotateRate * deltaTime * 100.f;
	return currentRotation;
}

void AFractalCircleMoveCutter::TransformCutterChildren(const FMatrix& parentMatrix, int depth, int& instanceIndex)
{
	if (depth <= 0)
	{
		return;
	}

	for (int i = 0; i < _param.childCountPerLayer; i++)
	{
		FScaleMatrix s (FVector(_param.sizeFactor));
		FRotationMatrix r (FRotator(0.f, 0,0.f));
		
		float sinValue, cosValue = 0.f;
		FMath::SinCos(&sinValue, &cosValue, 2 * PI / _param.childCountPerLayer * i);
		
		FTranslationMatrix t (FVector(_param.distanceFromParent * sinValue, _param.distanceFromParent * cosValue, 0.f));
		FMatrix childTransform = s * r * t;
		FMatrix childMatrix = childTransform * parentMatrix;
		_ismComponent->UpdateInstanceTransform(instanceIndex, FTransform(childMatrix), true, false, true);
		instanceIndex++;
		TransformCutterChildren(childMatrix, depth - 1, instanceIndex);
	}
}

void AFractalCircleMoveCutter::OnOverlapBreakableActor(AActor* otherActor)
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

void AFractalCircleMoveCutter::OnOverlapScoreTargetActor(AActor* otherActor) const
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

void AFractalCircleMoveCutter::OnOverlapDamageableActor(AActor* otherActor) const
{
	if (otherActor && otherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		UE_LOG(LogCutter, Log, TEXT("AddDamage %s by%s"), *GetName(), *otherActor->GetName());
		IDamageable::Execute_Damage(otherActor, _param.damage, GetActorLocation());
	}
}

void AFractalCircleMoveCutter::Break()
{
	UE_LOG(LogCutter, Log, TEXT("Break %s"), *GetName());
	if (IsValid(this))
	{
		OnBreak();
	}
}

void AFractalCircleMoveCutter::StartTargeting_Implementation(AActor* throwActor)
{
	check(IsValid(throwActor))
	FVector2D currentPos2D = FVector2D(GetActorLocation());
	FVector2D pointOfTangency = IStageShape::Execute_GetFarPointOfTangency(_stageShape.GetObject(), FVector2D(currentPos2D));
	FVector2D toStageCenterVec2D = (pointOfTangency - currentPos2D)/2; //円の端点2つ同士の距離から半径ベクトル導出 
	ResetTransformParam(pointOfTangency, toStageCenterVec2D);
	
	TObjectPtr<UCircleMoveTargetComponent> rotateTargetComponent = NewObject<UCircleMoveTargetComponent>(throwActor);
	FCircleMoveCutterThrowTargetParam throwTargetParam;

	//TODO: 現状左向き確定なので、左用の値を入れる
	//throwTargetParam.firstLookVec = FVector2D(toStageCenterVec2D);
	throwTargetParam.firstLookVec = FVector2D(toStageCenterVec2D.Y, -toStageCenterVec2D.X);
	
	throwTargetParam.rightMaxVec = FVector2D(toStageCenterVec2D.Y, -toStageCenterVec2D.X);
	throwTargetParam.leftMaxVec = FVector2D(-toStageCenterVec2D.Y, toStageCenterVec2D.X);
	rotateTargetComponent->RegisterParam(throwTargetParam, [this](const FCircleMoveCutterThrowParam& param){Throw(param);});

	//TODO: 実装完了次第消す
	UE_LOG(LogTemp, Log, TEXT("pointOfTangency: %s"), *pointOfTangency.ToString());
	UE_LOG(LogTemp, Log, TEXT("throwActor: %s, %p"), *throwActor->GetName(), throwActor);
	UE_LOG(LogTemp, Log, TEXT("throwTargetParam.firstLookVec : %s"), *throwTargetParam.firstLookVec.ToString());
	UE_LOG(LogTemp, Log, TEXT("throwTargetParam.rightMaxVec : %s"), *throwTargetParam.rightMaxVec.ToString());
	UE_LOG(LogTemp, Log, TEXT("throwTargetParam.leftLookVec : %s"), *throwTargetParam.leftMaxVec.ToString());
	
	rotateTargetComponent->RegisterComponent();
	rotateTargetComponent->Init();
}

void AFractalCircleMoveCutter::Throw(const FCircleMoveCutterThrowParam& param)
{
	UE_LOG(LogCutter, Log, TEXT("Throw %s"), *GetName());
	SetActorTickEnabled(true);
	OnThrown();
	PlayMoveStartAnimation();
	SetActorHiddenInGame(false);
}

void AFractalCircleMoveCutter::ResetTransformParam(FVector2D pointOfTangency, FVector2D toStageCenterVec2D)
{
	FVector2D currentPos2D = FVector2D(GetActorLocation());
	_rotateRadius = toStageCenterVec2D.Size();
	FVector2D rotateCenterPos2D = (pointOfTangency + currentPos2D) / 2;//円の端点2つ同士の中点から回転の中心点導出
	_rotateCenterPos = FVector(rotateCenterPos2D.X, rotateCenterPos2D.Y, IStageShape::Execute_GetCenterPos(_stageShape.GetObject()).Z);
	toStageCenterVec2D.Normalize();
	_currentAngle = FMath::Acos(FVector2D::DotProduct(FVector2D::UnitX(),-toStageCenterVec2D));//ベクトル同士の角度 = Acos(ベクトルの内積/各辺の大きさの積(Normalize済)なのでなし)
}

void AFractalCircleMoveCutter::OnBreak()
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