#pragma once

#include "StageShape.h"
#include "SquareStage.generated.h"

UCLASS()
class CUTTER_API ASquareStage : public AActor, public IStageShape
{
	GENERATED_BODY()
	
public:
	/*その点がステージの範囲内にあるかどうか*/
	virtual bool IsInStage_Implementation(FVector2D point) override;
	
	/* 指定の位置+方向ベクトルとステージの辺の交点情報（ステージの内部を通らない場合、FIntersectionDataのisInがfalseかつ交点情報は初期値が返る）*/
	virtual FIntersectionData GetInterSections_Implementation(FVector2D point, FVector2D vec) override;
	
	/*指定位置を通る円かつ、ステージ外にはみ出さずに一番大きく描ける内接円の中心座標を取得する(見つからなかった場合はoppositePointを返す)*/
	virtual FVector2D GetMaxSizeCircleCenterPos_Implementation(FVector2D oppositePoint) override;

protected:
	/*ステージのサイズ定義 (Min)(正方形想定)*/
	UFUNCTION(BlueprintImplementableEvent)
	FVector2D GetEdgeMin();
	
	/*ステージのサイズ定義 (Max)(正方形想定)*/
	UFUNCTION(BlueprintImplementableEvent)
	FVector2D GetEdgeMax();
	
private:
	/*stageEdge1,2を結ぶ線分との交点情報*/
	std::pair<bool, FVector2D> GetInterSection(FVector2D point, FVector2D vec, FVector2D stageEdge1, FVector2D stageEdge2);
	
	TArray<FVector2D> GetCircleCenterPos(FVector2D firstLine, FVector2D secondLine, FVector2D intersection, FVector2D oppositePoint);
	
	/* このステージで描ける最大の円の半径 */
	float GetMaxRadius();
};
