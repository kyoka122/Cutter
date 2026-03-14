#pragma once

#include "CoreMinimal.h"
#include "BaseObstacle.h"
#include "DataAsset/StraightRoundTripCutterDataAsset.h"
#include "GameFramework/Actor.h"
#include "StraightRoundTripCutter.generated.h"

UCLASS()
class CUTTER_API AStraightRoundTripCutter : public ABaseObstacle
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStraightRoundTripCutter(){};
	virtual void Tick(float DeltaTime) override;
	virtual FObstacleBaseParam GetBaseParam() const override { return param;}

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FStraightRoundTripCutterParam param = {};
	
private:
	void Init();
	void Translate(float deltaTime);
private:
};
