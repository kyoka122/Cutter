#pragma once

#include "StageShape.h"
#include "SquareStage.generated.h"

UCLASS()
class CUTTER_API ASquareStage : public AActor, public IStageShape
{
	GENERATED_BODY()
	
public:
	virtual bool IsInStage_Implementation(FVector2D point) override;
	virtual FIntersectionData GetInterSections_Implementation(FVector2D point, FVector2D vec) override;
	virtual FVector2D GetFarPointOfTangency_Implementation(FVector2D oppositePoint) override;

protected:
	/*ステージのサイズ定義 (Min)(正方形想定)*/
	UFUNCTION(BlueprintImplementableEvent)
	FVector2D GetEdgeMin();
	
	/*ステージのサイズ定義 (Max)(正方形想定)*/
	UFUNCTION(BlueprintImplementableEvent)
	FVector2D GetEdgeMax();
	
private:
	std::pair<bool, FVector2D> GetInterSection(FVector2D point, FVector2D vec, FVector2D stageEdge1, FVector2D stageEdge2);
	std::pair<bool, FVector2D> GetPointOfTangency(FVector2D point, FVector2D stageEdge1, FVector2D stageEdge2);
};
