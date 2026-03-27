#include "CutterBase.h"

ACutterBase::ACutterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACutterBase::StartTick()
{
	SetActorTickEnabled(true);
}

void ACutterBase::StopTick()
{
	SetActorTickEnabled(false);
}

void ACutterBase::RegisterScoreAddFunc(ScoreAddFunc func)
{
	_scoreAddFunc = func;
}

void ACutterBase::RegisterDeActiveFunc(TFunction<void()> deactiveFunc)
{
	_deactiveFunc = deactiveFunc;
}

void ACutterBase::RegisterStaticMeshEvent(UStaticMeshComponent* staticMeshComponent, OverlapFunc func)
{
	check(IsValid(staticMeshComponent));
	_overlapFunc = func;
	staticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ACutterBase::OnBeginOverlapEvent);
}

void ACutterBase::ReleaseStaticMeshEvent(UStaticMeshComponent* staticMeshComponent)
{
	check(IsValid(staticMeshComponent));
	_overlapFunc = nullptr;
	staticMeshComponent->OnComponentBeginOverlap.RemoveAll(this);
}

void ACutterBase::OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (_overlapFunc)
	{
		UE_LOG(LogTemp, Error, TEXT("実行する関数がnullです:"));
		return;
	}
	_overlapFunc(OtherActor);
};
