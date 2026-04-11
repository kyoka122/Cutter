#include "CutterBase.h"
#include "Cutter.h"
#include "Components/TimelineComponent.h"
#include "Struct/CutterBaseParam.h"

ACutterBase::ACutterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	_moveStartAnimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("MoveStartAnimTimeline"));
}

void ACutterBase::RegisterStageShapeData(TScriptInterface<IStageShape> stageShape)
{
	_stageShape = stageShape;
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
}

void ACutterBase::InitTimeline(UStaticMeshComponent* staticMeshComponent)
{
	check(IsValid(staticMeshComponent));
	_dynamicMaterial = staticMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
	check(IsValid(_dynamicMaterial));
	check(IsValid(_moveStartAnimTimeline));
	
	FOnTimelineFloat alphaUpdater;
	FOnTimelineFloat sizeUpdater;
	alphaUpdater.BindUFunction(this, "HandleBlinkUpdate");
	sizeUpdater.BindUFunction(this, "HandleSizeUpUpdate");
	_moveStartAnimTimeline->AddInterpFloat(_sizeUpCurve, sizeUpdater);
}

void ACutterBase::PlayMoveStartAnimation()
{
	check(IsValid(_moveStartAnimTimeline));
	_originSizeCache = GetActorScale3D();
	_moveStartAnimTimeline->SetTimelineLength(GetParam()->moveStartAnimationDuration);
	
	_moveStartAnimTimeline->PlayFromStart();
	GetWorldTimerManager().SetTimer(
		_startAnimationTimerHandle,
		[this]{OnEndMoveStartAnimation();},
		GetParam()->moveStartAnimationDuration,
		false);
}

void ACutterBase::OnEndMoveStartAnimation()
{
	HandleSizeUpUpdate(1);
	_moveStartAnimTimeline->Stop();
	GetWorldTimerManager().ClearTimer(_startAnimationTimerHandle);
	SetActorEnableCollision(true);
}


void ACutterBase::HandleSizeUpUpdate(float value)
{
	SetActorScale3D(_originSizeCache * value);
}

void ACutterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(_startAnimationTimerHandle);
}