#pragma once

#include "CoreMinimal.h"
#include "Obstacles/CutterBase.h"
#include "InGame/Interface/Breakable.h"
#include "Struct/CircleExpandCutterParam.h"
#include "CircleExpandCutter.generated.h"

/**
 * 円形ステージを円を描くように回りながら、中心から徐々に外に向かって円が広がっていくカッタークラス
 */
UCLASS()
class CUTTER_API ACircleExpandCutter : public ACutterBase, public IBreakable
{
	GENERATED_BODY()

public:
	ACircleExpandCutter(){}
	virtual void Tick(float DeltaTime) override;
	virtual void Break() override;
	virtual void ReStart() override;

protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FCircleExpandCutterParam _param;

private:
	void CheckOutOfStage();
	void Translate(float deltaTime);
	FVector CalcPosition(float deltaTime);
	FQuat CalcRotation(float deltaTime);
	void OnOverlapBreakableActor(AActor* otherActor);
	void OnOverlapScoreTargetActor(AActor* otherActor);
	void OnOverlapDamageableActor(AActor* otherActor);
	void LazyActiveStaticMeshEvent();
	void OnBreak();
	
private:
	float _currentAngle = 0.0f;
	FVector _rotateCenterPos = {};
	TObjectPtr<UStaticMeshComponent> _staticMeshComponent = {};
	FTimerHandle _overlapActiveTimerHandle = {};
};
