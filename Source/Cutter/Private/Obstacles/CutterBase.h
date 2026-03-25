#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CutterBase.generated.h"

UCLASS()
class CUTTER_API ACutterBase : public AActor
{
	GENERATED_BODY()
	
public:
	ACutterBase();
	virtual void BeginPlay() override;
	
	typedef TFunction<void(int)> ScoreAddFunc;
	void RegisterScoreAddFunc(ScoreAddFunc func);

protected:
	typedef TFunction<void(AActor*)> OverlapFunc;
	void RegisterStaticMeshEvent(UStaticMeshComponent* staticMeshComponent, OverlapFunc func);

protected:
	TArray<OverlapFunc> _overlapFunc = {};
	TFunction<void(int)> _scoreAddFunc = {};
	
private:
	UFUNCTION()
	virtual void OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
