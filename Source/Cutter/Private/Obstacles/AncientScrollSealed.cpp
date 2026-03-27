#include "AncientScrollSealed.h"

AAncientScrollSealed::AAncientScrollSealed()
{
	PrimaryActorTick.bCanEverTick = true;
}

int AAncientScrollSealed::RobbedScore_Implementation(bool isExecPlayer)
{
	if (_transformCutterFunc)
	{
		_transformCutterFunc(_type, GetActorTransform());
		//演出実行
		return _score;
	}
	return 0;
}

// void AAncientScrollActor::RegisterEvent(UStaticMeshComponent* staticMeshComponent,OverlapFunc func)
// {
// 	check(IsValid(staticMeshComponent));
// 	_overlapFunc.Add(func);
// 	staticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AAncientScrollActor::OnBeginOverlapEvent);
// }
//
// void AAncientScrollActor::RegisterStaticMeshEvent(UStaticMeshComponent* staticMeshComponent, OverlapFunc func)
// {
// 	check(IsValid(staticMeshComponent));
// 	_overlapFunc.Add(func);
// 	staticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AAncientScrollActor::OnBeginOverlapEvent);
// }
//
// void AAncientScrollActor::OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
// 		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	if (_overlapFunc.IsEmpty())
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("実行する関数がnullです:"));
// 	}
// 	for (auto& overlapFunc : _overlapFunc)
// 	{
// 		overlapFunc(OtherActor);
// 	}
// };