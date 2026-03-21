#pragma once

#include "CoreMinimal.h"
#include "Obstacles/CutterBase.h"
#include "Components/ActorComponent.h"
#include "DataAsset/CircleMoveCutterDataAsset.h"
#include "InGame/Interface/Breakable.h"
#include "InGame/Interface/Damageable.h"
#include "InGame/Interface/ScoreTarget.h"
#include "InGame/Interface/Throwable.h"
#include "CircleMoveCutter.generated.h"

/**
 * 円形ステージを円周上に周るカッタークラス
 */
UCLASS()
class CUTTER_API ACircleMoveCutter : public ACutterBase, public IThrowable, public IBreakable, public IScoreTarget
{
	GENERATED_BODY()

public:
	ACircleMoveCutter(){}
	virtual void Tick(float DeltaTime) override;
	virtual FCutterBaseParam GetBaseParam() const override { return _param;}
	virtual void Break() override;
	virtual void StartTargeting_Implementation() override;
	virtual void Throw_Implementation() override;
	virtual ECutterMode GetCurrentMode() override {return currentMode;}
	virtual int RobbedScore_Implementation(bool isExecPlayer) override;
	//virtual bool IsPlayerInteractable_Implementation() override {return _param.isPlayerInteractable;}
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FCircleMoveCutterParam _param = {};

private:
	void Init();
	void Translate(float deltaTime);
	FVector CalcPosition(float deltaTime);
	FQuat CalcRotation(float deltaTime);
	void OnOverlapBreakableActor(AActor* otherActor);
	void OnOverlapScoreTargetActor(AActor* otherActor);
	void OnOverlapDamageableActor(AActor* otherActor);

private:
	float _rotateRadius = 0.0f;
	float _currentAngle = 0.0f;
	FVector _rotateCenterPos = {};
	
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> _staticMeshComponent = {};
};