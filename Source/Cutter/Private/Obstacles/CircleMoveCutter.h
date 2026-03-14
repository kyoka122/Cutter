#pragma once

#include "CoreMinimal.h"
#include "Obstacles/BaseObstacle.h"
#include "Components/ActorComponent.h"
#include "DataAsset/CircleMoveCutterDataAsset.h"
#include "CircleMoveCutter.generated.h"

/**
 * 円形ステージを円周上に周るカッタークラス
 */
UCLASS()
class CUTTER_API ACircleMoveCutter : public ABaseObstacle
{
	GENERATED_BODY()

public:
	ACircleMoveCutter(){}
	virtual void Tick(float DeltaTime) override;
	virtual FObstacleBaseParam GetBaseParam() const override { return param;}

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FCircleMoveCutterParam param = {};
	
private:
	void Init();
	void Translate(float deltaTime);
	FVector CalcPosition(float deltaTime);
	FQuat CalcRotation(float deltaTime);
private:
	float _rotateRadius = 0.0f;
	float _currentAngle = 0.0f;
	FVector _rotateCenterPos = {};
};