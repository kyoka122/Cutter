#include "CutterBase.h"

ACutterBase::ACutterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACutterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACutterBase::RegisterScoreAddFunc(ScoreAddFunc func)
{
	_scoreAddFunc = func;
}

void ACutterBase::RegisterStaticMeshEvent(UStaticMeshComponent* staticMeshComponent, OverlapFunc func)
{
	check(IsValid(staticMeshComponent));
	_overlapFunc.Add(func);
	staticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ACutterBase::OnBeginOverlapEvent);
}

void ACutterBase::OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (_overlapFunc.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("実行する関数がnullです:"));
	}
	for (auto& overlapFunc : _overlapFunc)
	{
		overlapFunc(OtherActor);
	}
};
