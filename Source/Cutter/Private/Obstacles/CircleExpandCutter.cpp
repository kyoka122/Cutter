#include "CircleExpandCutter.h"

void ACircleExpandCutter::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void ACircleExpandCutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (currentMode == ECutterMode::Break)
	{
		Translate(DeltaTime);
	}
}

void ACircleExpandCutter::Init()
{
	FVector currentPos = GetActorLocation();
	_param.rotateCenterPos = currentPos;
	
	_staticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	check(IsValid(_staticMeshComponent));
	RegisterEvent(_staticMeshComponent, [this](AActor* otherActor)
	{
		OnOverlapBreakableActor(otherActor);
		OnOverlapScoreTargetActor(otherActor);
		OnOverlapDamageableActor(otherActor);
	});
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
	if (this < otherActor)//衝突した際片方が判定するため
	{
		return;
	}
	
	if (IBreakable* otherBreakable = Cast<IBreakable>(otherActor))
	{
		ECutterMode otherMode = otherBreakable->GetCurrentMode();
		if (currentMode == ECutterMode::Break && otherMode == ECutterMode::Break)
		{
			SetActorEnableCollision(false);
			otherActor->SetActorEnableCollision(false);
			UE_LOG(LogTemp, Log, TEXT("Destroy02,%s"), *otherActor->GetName());
			otherBreakable->Break();
			Destroy();
		}
		else if (currentMode == ECutterMode::Sphere && otherMode == ECutterMode::Break)//もし自分がCutterModeじゃないなら
		{
			UE_LOG(LogTemp, Log, TEXT("OwnBreak"));
			currentMode = ECutterMode::Break;
			//演出実行(変身も)
		}
	}
}

void ACircleExpandCutter::OnOverlapScoreTargetActor(AActor* otherActor)
{
	if (IScoreTarget* otherScoreTarget = Cast<IScoreTarget>(otherActor))
	{
		ECutterMode otherMode = otherScoreTarget->GetCurrentMode();
		if (currentMode == ECutterMode::Break && otherMode == ECutterMode::Sphere)//もし自分がCutterModeじゃないなら
		{
			UE_LOG(LogTemp, Log, TEXT("AddScore"));
			int score = otherScoreTarget->RobbedScore_Implementation(false);
			if (_scoreAddFunc)
			{
				_scoreAddFunc(score);
			}
			//演出実行
		}
	}
}

void ACircleExpandCutter::OnOverlapDamageableActor(AActor* otherActor)
{
	if (IDamageable* otherDamageable = Cast<IDamageable>(otherActor))
	{
		if (currentMode == ECutterMode::Break)
		{
			UE_LOG(LogTemp, Log, TEXT("AddDamage"));
			otherDamageable->Damage(_param.Damage, GetActorLocation());
			//演出実行
		}
	}
}

void ACircleExpandCutter::Break()
{
	UE_LOG(LogTemp, Log, TEXT("Imp_Destroy02"));
	if (IsValid(this))
	{
		Destroy();
	}
}

int ACircleExpandCutter::RobbedScore_Implementation(bool isExecPlayer)
{
	if (isExecPlayer)
	{
		return 0;
	}
	//演出実行
	if (currentMode == ECutterMode::Sphere)
	{
		currentMode = ECutterMode::Break;
		return _param.Score;
	} 
	return 0;
}
