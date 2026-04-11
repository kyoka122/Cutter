#pragma once
#include "StageShape.h"
#include "CircleStage.generated.h"

UCLASS()
class CUTTER_API ACircleStage : public AActor, public IStageShape
{
	GENERATED_BODY()
	
public:
	virtual bool IsInStage_Implementation(FVector2D point) override;
	virtual FIntersectionData GetInterSections_Implementation(FVector2D viaPoint, FVector2D vec) override;
	virtual FVector2D GetFarPointOfTangency_Implementation(FVector2D oppositePoint) override;
	
protected:
	/**
	 * ステージのサイズ
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Stage")
	float GetRadius();
};
