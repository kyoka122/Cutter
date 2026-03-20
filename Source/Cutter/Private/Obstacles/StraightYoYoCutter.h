#pragma once

#include "CoreMinimal.h"
#include "CutterBase.h"
#include "DataAsset/StraightRoundTripCutterDataAsset.h"
#include "GameFramework/Actor.h"
#include "InGame/Interface/Breakable.h"
#include "InGame/Interface/ScoreTarget.h"
#include "InGame/Interface/Throwable.h"
#include "StraightYoYoCutter.generated.h"

UCLASS()
class CUTTER_API AStraightYoYoCutter : public ACutterBase, public IThrowable, public IBreakable, public IScoreTarget
{
	GENERATED_BODY()

public:
	AStraightYoYoCutter(){}
	virtual void Tick(float DeltaTime) override;
	virtual FCutterBaseParam GetBaseParam() const override { return _param;}
	virtual void Break() override;
	virtual void OnStartTargeting() override;
	virtual void Throw() override;
	virtual ECutterMode GetCurrentMode() override {return currentMode;}
	virtual int RobbedScore_Implementation() override;
	virtual bool IsPlayerInteractable_Implementation() override {return _param.isPlayerInteractable;}
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FStraightRoundTripCutterParam _param = {};
	
private:
	void Init();
	void Translate(float deltaTime);
	void OnOverlapBreakableActor(AActor* otherActor);
	void OnOverlapScoreTargetActor(AActor* otherActor);
	
private:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> _staticMeshComponent = {};
};