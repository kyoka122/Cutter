#pragma once

#include "CoreMinimal.h"
#include "CutterBase.h"
#include "GameFramework/Actor.h"
#include "InGame/Interface/Breakable.h"
#include "InGame/Interface/Damageable.h"
#include "InGame/Interface/ScoreTarget.h"
#include "InGame/Interface/Throwable.h"
#include "Struct/StraightRoundTripCutterParam.h"
#include "StraightYoYoCutter.generated.h"

UCLASS()
class CUTTER_API AStraightYoYoCutter : public ACutterBase, public IThrowable, public IBreakable
{
	GENERATED_BODY()

public:
	AStraightYoYoCutter(){}
	virtual void Tick(float DeltaTime) override;
	virtual void Break() override;
	virtual void StartTargeting_Implementation() override;
	virtual void Throw_Implementation() override;
	virtual void Init(ScoreAddFunc* scoreAddFunc);
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FStraightRoundTripCutterParam _param = {};
	
private:
	void Translate(float deltaTime);
	void OnOverlapBreakableActor(AActor* otherActor);
	void OnOverlapScoreTargetActor(AActor* otherActor);
	void OnOverlapDamageableActor(AActor* otherActor);
	
private:
	bool hadThrew = false;
	TObjectPtr<UStaticMeshComponent> _staticMeshComponent = {};
};