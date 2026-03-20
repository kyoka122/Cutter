#pragma once

#include "CoreMinimal.h"
#include "Obstacles/CutterBase.h"
#include "DataAsset/CircleExpandCutterDataAsset.h"
//#include "InGame/EventRegistrants/BreakEventHelperOnBeginOverlap.h"
#include "InGame/EventRegistrants/OnBeginOverlapEventRegistrantBase.h"
#include "InGame/Interface/Breakable.h"
#include "InGame/Interface/ScoreTarget.h"
#include "CircleExpandCutter.generated.h"

/**
 * 円形ステージを円を描くように回りながら、中心から徐々に外に向かって円が広がっていくカッタークラス
 */
UCLASS()
class CUTTER_API ACircleExpandCutter : public ACutterBase, public IBreakable, public IScoreTarget
{
	GENERATED_BODY()

public:
	ACircleExpandCutter(){}
	virtual void Tick(float DeltaTime) override;
	virtual FCutterBaseParam GetBaseParam() const override { return FCutterBaseParam(_param);}
	virtual void Break() override;
	virtual ECutterMode GetCurrentMode() override {return currentMode;}
	virtual int RobbedScore_Implementation() override;
	virtual bool IsPlayerInteractable_Implementation() override {return _param.isPlayerInteractable;}
	
protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FCircleExpandCutterParam _param;

private:
	void Init();
	void Translate(float deltaTime);
	FVector CalcPosition(float deltaTime);
	FQuat CalcRotation(float deltaTime);
	void OnOverlapBreakableActor(AActor* otherActor);
	void OnOverlapScoreTargetActor(AActor* otherActor);
	
private:
	float _currentAngle = 0.0f;
	ECutterMode currentMode = {};
	
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> _staticMeshComponent = {};
};
