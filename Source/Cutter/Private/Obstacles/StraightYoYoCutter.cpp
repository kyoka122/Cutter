#include "StraightYoYoCutter.h"

#include "Cutter.h"
#include "InGame/Interface/Damageable.h"
#include "InGame/Interface/ScoreTarget.h"

void AStraightYoYoCutter::BeginPlay()
{
	Super::BeginPlay();
	_staticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
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
}

void AStraightYoYoCutter::OnOverlapBreakableActor(AActor* otherActor)
{
	if (IBreakable* otherBreakable = Cast<IBreakable>(otherActor))
	{
		UE_LOG(LogCutter, Log, TEXT("Break %s by%s"), *GetName(), *otherActor->GetName());
		otherBreakable->Break();
		OnBreak();
	}
}

void AStraightYoYoCutter::OnOverlapScoreTargetActor(AActor* otherActor)
{
	if (IScoreTarget* otherScoreTarget = Cast<IScoreTarget>(otherActor))
	{
		UE_LOG(LogCutter, Log, TEXT("AddScore %s by%s"), *GetName(), *otherActor->GetName());
		FScoreRobbedParam robbedParam = otherScoreTarget->RobbedScore_Implementation(false);
		if (robbedParam.canRobScore)
		{
			return;
		}
		if (!_scoreAddFunc)
		{
			UE_LOG(LogCutter, Error, TEXT("_scoreAddFunc 実行する関数がnullです %s"), *GetName());
			return;
		}
		_scoreAddFunc(robbedParam.score);
	}
}

void AStraightYoYoCutter::OnOverlapDamageableActor(AActor* otherActor)
{
	if (IDamageable* otherDamageable = Cast<IDamageable>(otherActor))
	{
		UE_LOG(LogCutter, Log, TEXT("AddDamage %s by%s"), *GetName(), *otherActor->GetName());
		otherDamageable->Damage(_param.Damage, GetActorLocation());
		//演出実行
	}
}

void AStraightYoYoCutter::Break()
{
	UE_LOG(LogCutter, Log, TEXT("Imp_Destroy %s"), *GetName());
	if (IsValid(this))
	{
		OnBreak();
	}
}

void AStraightYoYoCutter::StartTargeting_Implementation()
{
	UE_LOG(LogCutter, Log, TEXT("PrepareThrow %s"), *GetName());
}

void AStraightYoYoCutter::Throw_Implementation()
{
	UE_LOG(LogCutter, Log, TEXT("Throw %s"), *GetName());
	
	StartTick();
	SetActorHiddenInGame(false);
	LazyActiveStaticMeshEvent();
}

void AStraightYoYoCutter::LazyActiveStaticMeshEvent()
{
	GetWorldTimerManager().SetTimer(
		_overlapActiveTimerHandle,
		[this]{SetActorEnableCollision(true);},
		1.0f,
		false
	);
}

void AStraightYoYoCutter::OnBreak()
{
	SetActorEnableCollision(false);
	StopTick();
	SetActorHiddenInGame(true);
	if (!_inactiveFunc)
	{
		UE_LOG(LogCutter, Error, TEXT("_inactiveFunc 実行する関数がnullです %s"), *GetName());
		return;
	}
	_inactiveFunc();
}