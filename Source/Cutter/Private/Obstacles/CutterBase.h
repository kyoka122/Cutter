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
	
	void StartTick();
	void StopTick();
	virtual void ReStart(){}
	typedef TFunction<void(int)> ScoreAddFunc;
	void RegisterScoreAddFunc(ScoreAddFunc func);
	void RegisterDeActiveFunc(TFunction<void()> _destroyFunc);
	
protected:
	typedef TFunction<void(AActor*)> OverlapFunc;
	void RegisterStaticMeshEvent(UStaticMeshComponent* staticMeshComponent, OverlapFunc func);

protected:
	OverlapFunc _overlapFunc = {};
	ScoreAddFunc _scoreAddFunc = {};
	TFunction<void()> _deactiveFunc = {};
	
private:
	UFUNCTION()
	virtual void OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
