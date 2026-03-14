#pragma once

#include "CoreMinimal.h"
#include "Obstacles/BaseObstacle.h"
#include "Components/ActorComponent.h"
#include "DataAsset/CircleExpandCutterDataAsset.h"
#include "CircleExpandCutter.generated.h"

/**
 * 円形ステージを円を描くように回りながら、中心から徐々に外に向かって円が広がっていくカッタークラス
 */
UCLASS()
class CUTTER_API ACircleExpandCutter : public ABaseObstacle
{
	GENERATED_BODY()

public:
	ACircleExpandCutter(){}
	virtual void Tick(float DeltaTime) override;
	virtual FObstacleBaseParam GetBaseParam() const override { return FObstacleBaseParam(param);}

protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FCircleExpandCutterParam param;
	
private:
	void Init();
	void Translate(float deltaTime);
	FVector CalcPosition(float deltaTime);
	FQuat CalcRotation(float deltaTime);

private:
	float _currentAngle = 0.0f;
};
