#pragma once

#include "CoreMinimal.h"
#include "Struct/IntersectionData.h"
#include "StageShape.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UStageShape : public UInterface
{
	GENERATED_BODY()
};

class CUTTER_API IStageShape
{
	GENERATED_BODY()
	
public:
	
	/*その点がステージの範囲内にあるかどうか*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stage")
	bool IsInStage(FVector2D point);
	/*その点がステージの範囲内にあるかどうか*/
	virtual bool IsInStage_Implementation(FVector2D point) = 0;
	
	/**
	 * そのベクトルとの交点
	 * （ステージの内部を通らない場合、FIntersectionDataのisInがfalseかつ交点情報は初期値が返る）
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stage")
	FIntersectionData GetInterSections(FVector2D point, FVector2D vec);
	/**
	 * そのベクトルとの交点
	 * （ステージの内部を通らない場合、FIntersectionDataのisInがfalseかつ交点情報は初期値が返る）
	 */
	virtual FIntersectionData GetInterSections_Implementation(FVector2D point, FVector2D vec) = 0;
	
	/*内接円を作った時の接点（現状は一番大きい内接円を描いた時の接点）*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Stage")
	FVector2D GetFarPointOfTangency(FVector2D oppositePoint);
	/* 内接円を作った時の接点（現状は一番大きい内接円を描いた時の接点）*/
	virtual FVector2D GetFarPointOfTangency_Implementation(FVector2D oppositePoint) = 0;
	
	/*ステージの中心点*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Stage")
	FVector GetCenterPos();
	
};