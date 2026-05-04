#include "CutterBlower.h"

#include "Cutter.h"
#include "InGame/Interface/Breakable.h"
#include "InGame/Stage/StageShape.h"

class IBreakable;

ACutterBlower::ACutterBlower()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACutterBlower::BeginPlay()
{
	Super::BeginPlay();
	_staticMeshComponent = GetStaticMesh();
	if (!IsValid(_staticMeshComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("_staticMeshComponentが取得できませんでした。"));
		return;
	}
	_staticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ACutterBlower::OnBeginOverlapEvent);
}

void ACutterBlower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move(DeltaTime);
	
	if (_param.stageShape)
	{
		if (!IStageShape::Execute_IsInStage(_param.stageShape.GetObject(), FVector2D(GetActorLocation())))
		{
			OnBreak();
		}
	}
	else UE_LOG(LogCutter, Error, TEXT("_param.stageShapeがnullです %s"), *GetName());
}

void ACutterBlower::RegisterMoveParam(const FCutterBlowParam& param)
{
	_param = param;
}

void ACutterBlower::Move(float deltaTime)
{
	FVector2D delta = _param.moveDirection * moveSpeed * deltaTime;
	AddActorWorldOffset(FVector(delta.X, delta.Y, 0.0f));
}

void ACutterBlower::OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnOverlapBreakable(OtherActor);
}

void ACutterBlower::OnOverlapBreakable(AActor* otherActor) const
{
	if (IBreakable* otherBreakable = Cast<IBreakable>(otherActor))
	{
		UE_LOG(LogCharacter, Log, TEXT("Break %s by%s"), *GetName(), *otherActor->GetName());
		otherBreakable->Break();
	}
}

void ACutterBlower::OnBreak()
{
	Destroy();
}