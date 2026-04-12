#pragma once

#include "CoreMinimal.h"
#include "Obstacles/Cutters/CutterBase.h"
#include "InGame/Interface/Breakable.h"
#include "InGame/Interface/Throwable.h"
#include "Struct/CircleMoveCutterParam.h"
#include "CircleMoveCutter.generated.h"

/**
 * 円形ステージを円周上に周るカッタークラス
 */
UCLASS()
class CUTTER_API ACircleMoveCutter : public ACutterBase, public IThrowable, public IBreakable
{
	GENERATED_BODY()

public:
	ACircleMoveCutter(){}
	virtual void Tick(float DeltaTime) override;
	void ResetTransformParam();
	virtual void Break() override;
	virtual void StartTargeting_Implementation(AActor* throwActor) override;
	virtual void Throw_Implementation() override;
	virtual FVector GetLocation_Implementation() const override { return GetActorLocation(); };

protected:
	virtual void BeginPlay() override;
	virtual FCutterBaseParam* GetParam() override { return &_param; }
	
protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FCircleMoveCutterParam _param = {};

private:
	void Translate(float deltaTime);
	FVector CalcPosition(float deltaTime);
	FRotator CalcRotation(float deltaTime) const;
	void OnOverlapBreakableActor(AActor* otherActor);
	void OnOverlapScoreTargetActor(AActor* otherActor) const;
	void OnOverlapDamageableActor(AActor* otherActor) const;
	void OnBreak();
	
private:
	float _rotateRadius = 0.f;
	float _currentAngle = 0.f;
	FVector _rotateCenterPos = {};
	FVector2D _toStageCenterVec2D = {};
	
	UPROPERTY() TObjectPtr<UStaticMeshComponent> _staticMeshComponent = {};
};