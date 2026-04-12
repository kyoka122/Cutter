#include "SquareStage.h"

bool ASquareStage::IsInStage_Implementation(FVector2D point)
{
	return FBox2D(GetEdgeMin(),GetEdgeMax()).IsInsideOrOn(point);
}

FIntersectionData ASquareStage::GetInterSections_Implementation(FVector2D point, FVector2D vec)
{
	FBox2D stageEdge = FBox2D(GetEdgeMin(),GetEdgeMax());
	FIntersectionData intersectionData;
	TArray<std::pair<bool, FVector2D>> results;
	results.Add(GetInterSection(point, vec, FVector2D(stageEdge.Min.X, stageEdge.Min.Y), FVector2D(stageEdge.Min.X, stageEdge.Max.Y)));
	results.Add(GetInterSection(point, vec, FVector2D(stageEdge.Min.X, stageEdge.Max.Y), FVector2D(stageEdge.Max.X, stageEdge.Max.Y)));
	results.Add(GetInterSection(point, vec, FVector2D(stageEdge.Max.X, stageEdge.Max.Y), FVector2D(stageEdge.Max.X, stageEdge.Min.Y)));
	results.Add(GetInterSection(point, vec, FVector2D(stageEdge.Max.X, stageEdge.Min.Y), FVector2D(stageEdge.Min.X, stageEdge.Min.Y)));
	for (const auto& result : results)//TODO: イテレータにしたい
	{
		if (!result.first)
		{
			continue;
		}
		if (!intersectionData.isIn)
		{
			//初めて交点を見つけた時
			intersectionData.isIn = true;
			intersectionData.point1 = result.second;
			continue;
		}
		//point1と同じ値でないかどうか確認する。(頂点と交差する場合、同じ頂点を持つ2辺と交わる可能性があり、同じ頂点が2つ導出されている可能性があるため)
		if (!FMath::IsNearlyEqual(FVector2D::DotProduct(intersectionData.point1, result.second), 1.f))
		{
			if (!intersectionData.isTangent)
			{
				//2つ目の交点を見つけた時
				intersectionData.isTangent = true;
				intersectionData.point2 = result.second;
				continue;
			}
			UE_LOG(LogTemp, Warning, TEXT("3つor4つ目の交点を見つけました。 min: %s, max: %s"), *stageEdge.Min.ToString(), *stageEdge.Max.ToString());
		}
	}
	if (FMath::IsNearlyEqual(FVector2D::DotProduct(vec, intersectionData.point2), 1.f))
	{
		Swap(intersectionData.point1, intersectionData.point2);
	}
	return intersectionData;
}

FVector2D ASquareStage::GetFarPointOfTangency_Implementation(FVector2D oppositePoint)
{
	FBox2D stageEdge = FBox2D(GetEdgeMin(),GetEdgeMax());
	
	TArray<std::pair<bool, FVector2D>> results;
	results.Add(GetPointOfTangency(oppositePoint, FVector2D(stageEdge.Min.X, stageEdge.Min.Y), FVector2D(stageEdge.Min.X, stageEdge.Max.Y)));
	results.Add(GetPointOfTangency(oppositePoint, FVector2D(stageEdge.Min.X, stageEdge.Max.Y), FVector2D(stageEdge.Max.X, stageEdge.Max.Y)));
	results.Add(GetPointOfTangency(oppositePoint, FVector2D(stageEdge.Max.X, stageEdge.Max.Y), FVector2D(stageEdge.Max.X, stageEdge.Min.Y)));
	results.Add(GetPointOfTangency(oppositePoint, FVector2D(stageEdge.Max.X, stageEdge.Min.Y), FVector2D(stageEdge.Min.X, stageEdge.Min.Y)));

	FVector2D farTangency = FVector2D(results[0].second);
	for (const auto& result : results)
	{
		if (!result.first)
		{
			UE_LOG(LogTemp, Log, TEXT("交点のない辺がありました。: %s, point位置: %s"), *result.second.ToString(), *oppositePoint.ToString());
			continue;
		}
		if ((result.second - oppositePoint).Length() > (farTangency - oppositePoint).Length())
		{
			farTangency = result.second;
		}
	}
	return farTangency;
}

std::pair<bool, FVector2D> ASquareStage::GetPointOfTangency(FVector2D point, FVector2D stageEdge1, FVector2D stageEdge2)
{
	std::pair result = {false, FVector2D(0.0f, 0.0f)};
	
	FVector2D stageLine = stageEdge2 - stageEdge1;
	FVector2D pointToStageEdge1 = stageEdge1 - point;
	
	//MEMO:  FVector2.DotProduct (pointToStageEdge1 + t * stageLine , stageLine) = 0 （直交する）を変形
	float t = - (pointToStageEdge1.X * stageLine.X + pointToStageEdge1.Y * stageLine.Y) / (stageLine.X * stageLine.X + stageLine.Y * stageLine.Y);
	
	if (t >= 0 && t <= 1)
	{
		result.first = true;
		result.second = stageEdge1 + t * stageLine;
	}
	return result;
}

std::pair<bool, FVector2D> ASquareStage::GetInterSection(FVector2D point, FVector2D vec, FVector2D stageEdge1, FVector2D stageEdge2)
{
	std::pair result = {false, FVector2D(0.0f, 0.0f)};
	
	if (FMath::IsNearlyZero(vec.Length()))
	{
		return result;
	}
	
	float cross1 = FVector2D::CrossProduct(stageEdge1 - point, vec);
	float cross2 = FVector2D::CrossProduct(vec, stageEdge2 - stageEdge1);
	if (FMath::IsNearlyZero(cross2))
	{
		return result;//直線に対してvecが平行 or 線分の大きさが0の時
	}
	
	float t = cross1 / cross2;
	if (t >= 0 && t <= 1)
	{
		result.first = true;
		result.second = stageEdge1 + t * (stageEdge2 - stageEdge1);
	}
	return result;
}