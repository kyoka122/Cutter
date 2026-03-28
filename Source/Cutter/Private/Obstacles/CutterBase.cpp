#include "CutterBase.h"

#include "Cutter.h"

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

void ACutterBase::RegisterParam(TObjectPtr<FStageEnvironmentParam> stageEnvironmentParam)
{
	_stageEnvironmentParam = stageEnvironmentParam;
}

void ACutterBase::RegisterScoreAddFunc(ScoreAddFunc func)
{
	_scoreAddFunc = func;
}

void ACutterBase::RegisterInactiveFunc(TFunction<void()> inactiveFunc)
{
	_inactiveFunc = inactiveFunc;
}

void ACutterBase::RegisterStaticMeshEvent(UStaticMeshComponent* staticMeshComponent, OverlapFunc func)
{
	check(IsValid(staticMeshComponent));
	_overlapFunc = func;
	staticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ACutterBase::OnBeginOverlapEvent);
}

void ACutterBase::OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!_overlapFunc)
	{
		UE_LOG(LogCutter, Error, TEXT("_overlapFunc 実行する関数がnullです %s by%s"), *GetName(), *OtherActor->GetName());
		return;
	}
	_overlapFunc(OtherActor);
};
