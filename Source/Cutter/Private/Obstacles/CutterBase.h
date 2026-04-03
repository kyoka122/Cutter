#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CutterBase.generated.h"

struct  FStageEnvironmentParam;

UCLASS()
class CUTTER_API ACutterBase : public AActor
{
	GENERATED_BODY()
	
public:
	ACutterBase();
	
	virtual void ReStart(){}
	void RegisterParam(TObjectPtr<FStageEnvironmentParam> stageEnvironmentParam);
	using ScoreAddFunc = TFunction<void(int)>;
	void RegisterScoreAddFunc(ScoreAddFunc func);
	void RegisterInactiveFunc(TFunction<void()> _inactiveFunc);
	
protected:
	using OverlapFunc = TFunction<void(AActor*)>;
	void RegisterStaticMeshEvent(UStaticMeshComponent* staticMeshComponent, OverlapFunc func);

protected:
	OverlapFunc _overlapFunc = {};
	ScoreAddFunc _scoreAddFunc = {};
	TFunction<void()> _inactiveFunc = {};
	TObjectPtr<FStageEnvironmentParam> _stageEnvironmentParam = {};
	
private:
	UFUNCTION()
	virtual void OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
