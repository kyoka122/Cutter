#include "StraightYoYoCutter.h"
#include "InGame/Interface/Damageable.h"
#include "InGame/Interface/ScoreTarget.h"

void AStraightYoYoCutter::BeginPlay()
{
	Super::BeginPlay();
	_staticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	check(IsValid(_staticMeshComponent));
}

void AStraightYoYoCutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Translate(DeltaTime);
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
		OnBreak();
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
		OnBreak();
	}
}

void AStraightYoYoCutter::StartTargeting_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("PrepareThrow"));
}

void AStraightYoYoCutter::Throw_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Throw 02"));
	RegisterStaticMeshEvent(_staticMeshComponent, [this](AActor* otherActor)
	{
		OnOverlapBreakableActor(otherActor);
		OnOverlapScoreTargetActor(otherActor);
		OnOverlapDamageableActor(otherActor);
	});
	StartTick();
	SetActorHiddenInGame(false);
}

void AStraightYoYoCutter::OnBreak()
{
	ReleaseStaticMeshEvent(_staticMeshComponent);
	StopTick();
	SetActorHiddenInGame(true);
	if (_deactiveFunc)
	{
		_deactiveFunc();
	}
}