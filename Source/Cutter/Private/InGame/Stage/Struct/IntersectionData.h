#pragma once
#include "IntersectionData.generated.h"

/**
 * IStageで使用。線分とステージ外枠との交点を求めた時の結果を表す構造体。
 */
USTRUCT(BlueprintType)
struct FIntersectionData
{
	GENERATED_BODY()
public:
	/*そもそも点がステージの中にあるかどうか*/
	bool isIn = false;
	/*点が接する場合(float値での計算のためめったに起こらない。この場合、point1,2には同じ値が入る)*/
	bool isTangent = false;
	FVector2D point1 = {};
	FVector2D point2 = {};
};
