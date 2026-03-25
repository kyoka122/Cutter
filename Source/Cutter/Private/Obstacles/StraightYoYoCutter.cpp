#include "StraightYoYoCutter.h"

void AStraightYoYoCutter::BeginPlay()
{
	Super::BeginPlay();
}

void AStraightYoYoCutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (hadThrew)
	{
		Translate(DeltaTime);
	}
}

void AStraightYoYoCutter::Init(ScoreAddFunc* scoreAddFunc)
{
	_staticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	check(IsValid(_staticMeshComponent));
	RegisterStaticMeshEvent(_staticMeshComponent, [this](AActor* otherActor)
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
	if (IBreakable* otherBreakable = Cast<IBreakable>(otherActor))
	{
		SetActorEnableCollision(false);
		otherActor->SetActorEnableCollision(false);
		UE_LOG(LogTemp, Log, TEXT("Destroy03,%s"), *otherActor->GetName());
		otherBreakable->Break();
		Destroy();
	}
}

void AStraightYoYoCutter::OnOverlapScoreTargetActor(AActor* otherActor)
{
	if (IScoreTarget* otherScoreTarget = Cast<IScoreTarget>(otherActor))
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

void AStraightYoYoCutter::OnOverlapDamageableActor(AActor* otherActor)
{
	if (IDamageable* otherDamageable = Cast<IDamageable>(otherActor))
	{
		UE_LOG(LogTemp, Log, TEXT("AddDamage"));
		otherDamageable->Damage(_param.Damage, GetActorLocation());
		//演出実行
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
	hadThrew = true;
}