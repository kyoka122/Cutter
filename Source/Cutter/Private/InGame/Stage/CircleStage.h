#pragma once

#include "StageShape.h"
#include "CircleStage.generated.h"

/*
 * 円形ステージのステージ情報
 */
UCLASS()
class CUTTER_API ACircleStage : public AActor, public IStageShape
{
	GENERATED_BODY()
	
public:
	/*その点がステージの範囲内にあるかどうか*/
	virtual bool IsInStage_Implementation(FVector2D point) override;
	
	/*指定の位置+方向ベクトルとステージの辺の交点情報（ステージの内部を通らない場合、FIntersectionDataのisInがfalseかつ交点情報は初期値が返る）*/
	virtual FIntersectionData GetInterSections_Implementation(FVector2D point, FVector2D vec) override;
	
	/*指定位置を通る円かつ、ステージ外にはみ出さずに一番大きく描ける内接円の接点座標*/
	virtual FVector2D GetMaxSizeCircleCenterPos_Implementation(FVector2D oppositePoint) override;
	
protected:
	/*ステージの半径*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Stage")
	float GetRadius();
};
