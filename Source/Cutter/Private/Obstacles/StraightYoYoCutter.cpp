#include "StraightYoYoCutter.h"

#include "Cutter.h"
#include "ActorComponenet/FullRotateTargetComponent.h"
#include "InGame/Interface/Damageable.h"
#include "InGame/Interface/ScoreTarget.h"
#include "InGame/Stage/StageShape.h"

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
	if (otherActor && otherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		UE_LOG(LogCutter, Log, TEXT("AddDamage %s by%s"), *GetName(), *otherActor->GetName());
		IDamageable::Execute_Damage(otherActor, _param.Damage, GetActorLocation());
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

void AStraightYoYoCutter::StartTargeting_Implementation(AActor* throwActor)
{
	check(throwActor);
	UE_LOG(LogCutter, Log, TEXT("PrepareThrow %s"), *GetName());
	UFullRotateTargetComponent* fullRotateTargetComponent = NewObject<UFullRotateTargetComponent>(throwActor);

	fullRotateTargetComponent->RegisterThrowable(this);
	fullRotateTargetComponent->RegisterComponent();
	fullRotateTargetComponent->Init();
}

void AStraightYoYoCutter::Throw_Implementation()
{
	UE_LOG(LogCutter, Log, TEXT("Throw %s"), *GetName());
	
	SetActorTickEnabled(true);
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
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	if (!_inactiveFunc)
	{
		UE_LOG(LogCutter, Error, TEXT("_inactiveFunc 実行する関数がnullです %s"), *GetName());
		return;
	}
	_inactiveFunc();
}