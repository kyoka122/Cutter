#include "CircleMoveCutter.h"
#include "HAL/PreprocessorHelpers.h"

void ACircleMoveCutter::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void ACircleMoveCutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (currentMode == ECutterMode::Break)
	{
		Translate(DeltaTime);
	}
}

void ACircleMoveCutter::Init()
{
	_staticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	check(IsValid(_staticMeshComponent));
	RegisterEvent(_staticMeshComponent, [this](AActor* otherActor)
	{
		OnOverlapBreakableActor(otherActor);
		OnOverlapScoreTargetActor(otherActor);
		OnOverlapDamageableActor(otherActor);
	});
	
	FVector currentPos = GetActorLocation();
	float radius = _param.stageSize / 2.f - FMath::Abs(currentPos.X / 2);
	FVector toStageCenterVec = _param.stageCenterPos - currentPos;
	FVector toStageCenterVec2D = FVector(toStageCenterVec.X, toStageCenterVec.Y, 0);
	_rotateRadius = (toStageCenterVec2D.Size() + radius) / 2;
	toStageCenterVec2D.Normalize();
	_rotateCenterPos = currentPos + toStageCenterVec2D * _rotateRadius;
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
		ECutterMode otherMode = otherBreakable->GetCurrentMode();
		if (currentMode == ECutterMode::Break && otherMode == ECutterMode::Break)
		{
			SetActorEnableCollision(false);
			otherActor->SetActorEnableCollision(false);
			UE_LOG(LogTemp, Log, TEXT("Destroy01,%s"), *otherActor->GetName());
			otherBreakable->Break();
			Destroy();
		}
		// else if (currentMode == ECutterMode::Sphere && otherMode == ECutterMode::Break)//もし自分がCutterModeじゃないなら
		// {
		// 	UE_LOG(LogTemp, Log, TEXT("OwnBreak"));
		// 	currentMode = ECutterMode::Break;
		// 	//演出実行(変身も)
		// }
	}
}

void ACircleMoveCutter::OnOverlapScoreTargetActor(AActor* otherActor)
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

void ACircleMoveCutter::OnOverlapDamageableActor(AActor* otherActor)
{
	if (currentMode != ECutterMode::Break)
	{
		return;
	}
	if (otherActor && otherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("AddDamage"));
		IDamageable::Execute_Damage(otherActor, _param.Damage, GetActorLocation());
	}else
	{
		UE_LOG(LogTemp, Log, TEXT("MissDamage"));
	}
}

void ACircleMoveCutter::Break()
{
	UE_LOG(LogTemp, Log, TEXT("Imp_Destroy01"));
	if (IsValid(this))
	{
		Destroy();
	}
}

void ACircleMoveCutter::StartTargeting_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("PrepareThrow"));
}

void ACircleMoveCutter::Throw_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Throw"));
	
	currentMode = ECutterMode::Break;
}

int ACircleMoveCutter::RobbedScore_Implementation(bool isExecPlayer)
{
	//演出実行
	if (currentMode == ECutterMode::Sphere)
	{
		currentMode = ECutterMode::Translating;
		return _param.Score;
	} 
	return 0;
}