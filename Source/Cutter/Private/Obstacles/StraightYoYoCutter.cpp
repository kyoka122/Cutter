#include "StraightYoYoCutter.h"

// Called when the game starts or when spawned
void AStraightYoYoCutter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStraightYoYoCutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (currentMode == ECutterMode::Break)
	{
		Translate(DeltaTime);
	}
}

void AStraightYoYoCutter::Init()
{
	_staticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	check(IsValid(_staticMeshComponent));
	RegisterEvent(_staticMeshComponent, [this](AActor* otherActor)
	{
		OnOverlapBreakableActor(otherActor);
		OnOverlapScoreTargetActor(otherActor);
		OnOverlapDamageableActor(otherActor);
	});
}

void AStraightYoYoCutter::Translate(float deltaTime)
{
}

void AStraightYoYoCutter::OnOverlapBreakableActor(AActor* otherActor)
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
			UE_LOG(LogTemp, Log, TEXT("Destroy03,%s"), *otherActor->GetName());
			otherBreakable->Break();
			Destroy();
		}
	}
}

void AStraightYoYoCutter::OnOverlapScoreTargetActor(AActor* otherActor)
{
	if (IScoreTarget* otherScoreTarget = Cast<IScoreTarget>(otherActor))
	{
		ECutterMode otherMode = otherScoreTarget->GetCurrentMode();
		if (currentMode == ECutterMode::Break && otherMode == ECutterMode::Sphere)
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

void AStraightYoYoCutter::OnOverlapDamageableActor(AActor* otherActor)
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

void AStraightYoYoCutter::Break()
{
	UE_LOG(LogTemp, Log, TEXT("Imp_Destroy03"));
	if (IsValid(this))
	{
		Destroy();
	}
}

void AStraightYoYoCutter::StartTargeting_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("PrepareThrow"));
}

void AStraightYoYoCutter::Throw_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Throw"));
	currentMode = ECutterMode::Break;
}

int AStraightYoYoCutter::RobbedScore_Implementation(bool isExecPlayer)
{
	//演出実行
	if (currentMode == ECutterMode::Sphere)
	{
		currentMode = ECutterMode::Translating;
		return _param.Score;
	} 
	return 0;
}