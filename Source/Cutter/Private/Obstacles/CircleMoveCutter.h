#pragma once

#include "CoreMinimal.h"
#include "Obstacles/CutterBase.h"
#include "Components/ActorComponent.h"
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

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FCircleMoveCutterParam _param = {};

private:
	void Translate(float deltaTime);
	FVector CalcPosition(float deltaTime);
	FRotator CalcRotation(float deltaTime);
	void OnOverlapBreakableActor(AActor* otherActor);
	void OnOverlapScoreTargetActor(AActor* otherActor);
	void OnOverlapDamageableActor(AActor* otherActor);
	void LazyActiveStaticMeshEvent();
	void OnBreak();
	
private:
	TObjectPtr<UStaticMeshComponent> _staticMeshComponent = {};
	FTimerHandle _overlapActiveTimerHandle = {};
	float _rotateRadius = 0.0f;
	float _currentAngle = 0.0f;
	FVector _rotateCenterPos = {};
	FVector2D _toStageCenterVec2D = {};//Zが0のベクトル
};