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
	for (const auto& result : results)
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
	float point2Product = FVector2D::DotProduct(vec, intersectionData.point2 - point);
	if (point2Product > 0)//MEMO: >0の時、だいたい同じ方向を向いている（誤差90°以内）
	{
		Swap(intersectionData.point1, intersectionData.point2);
	}
	return intersectionData;
}

FVector2D ASquareStage::GetMaxSizeCircleCenterPos_Implementation(FVector2D oppositePoint)
{
	FBox2D edge = FBox2D(GetEdgeMin(),GetEdgeMax());//TODO: FBox2DをFVector2Dを4つに直す
	
	TArray<std::pair<bool, FVector2D>> results;
	std::pair<bool, FVector2D> under = GetPointOfTangency(oppositePoint, FVector2D(edge.Min.X, edge.Min.Y), FVector2D(edge.Min.X, edge.Max.Y));//→
	std::pair<bool, FVector2D> right = GetPointOfTangency(oppositePoint, FVector2D(edge.Min.X, edge.Max.Y), FVector2D(edge.Max.X, edge.Max.Y));//↑
	std::pair<bool, FVector2D> upper = GetPointOfTangency(oppositePoint, FVector2D(edge.Max.X, edge.Max.Y), FVector2D(edge.Max.X, edge.Min.Y));//←
	std::pair<bool, FVector2D> left = GetPointOfTangency(oppositePoint, FVector2D(edge.Max.X, edge.Min.Y), FVector2D(edge.Min.X, edge.Min.Y));//↓
	
	//□のどの二辺を選ぶと一番大きな円を描けるか取得
	bool underIsFar = CompareFarPoint(under, upper, oppositePoint);
	bool rightIsFar = CompareFarPoint(right, left, oppositePoint);
	
	//選ばれた二辺の情報をキャッシュ
	FVector2D firstLine;
	FVector2D secondLine;
	FVector2D intersection;
	
	//正方形であれば90°で問題ないので↓は必要ない
	if (underIsFar && rightIsFar)
	{
		UE_LOG(LogTemp, Log, TEXT("under, right"));
		firstLine = FVector2D(0, edge.Min.Y - edge.Max.Y);//under
		secondLine = FVector2D(edge.Max.X - edge.Min.X, 0);//right
		intersection = FVector2D(edge.Min.X, edge.Max.Y);
	}
	else if (underIsFar && !rightIsFar)
	{
		UE_LOG(LogTemp, Log, TEXT("under, left"));
		firstLine = FVector2D(0, edge.Max.Y - edge.Min.Y);//under
		secondLine = FVector2D(edge.Max.X - edge.Min.X, 0);//left
		intersection = FVector2D(edge.Min.X, edge.Min.Y);
	}
	else if (!underIsFar && rightIsFar)
	{
		UE_LOG(LogTemp, Log, TEXT("upper, right"));
		firstLine = FVector2D(0, edge.Min.Y - edge.Max.Y);//upper
		secondLine = FVector2D(edge.Min.X - edge.Max.X, 0);//right
		intersection = FVector2D(edge.Max.X, edge.Max.Y);
	}
	else if (!underIsFar && !rightIsFar)
	{
		UE_LOG(LogTemp, Log, TEXT("upper, left"));
		firstLine = FVector2D(0, edge.Max.Y - edge.Min.Y);//upper
		secondLine = FVector2D(edge.Min.X - edge.Max.X, 0);//left
		intersection = FVector2D(edge.Max.X, edge.Min.Y);
	}
	
	//「1. rの長さと、二辺の交点～円の中心までの関係性」と、「2. oppositePoint～円の中心までの距離の関係性」を　元に連立方程式を解く
	// oppositePointをP, 二辺の交点をQ、円の中心をOとすると...
	
	//1. rの長さと、二辺の交点～円の中心までの関係性: PO→ = r/sin(θ/2) * (firstLine.Normalize() + secondLine.Normalize()) + PQ→
	//2. oppositePoint～円の中心までの距離の関係性: |OP→| = r
	
	float twoLineHalfRadian = FMath::Acos(FVector2D::DotProduct(firstLine, secondLine) / (firstLine.Length() * secondLine.Length())) / 2;
	FVector2D twoLineCenterVec = (firstLine.GetSafeNormal() + secondLine.GetSafeNormal()).GetSafeNormal();
	
	float a = 1 / FMath::Square(FMath::Sin(twoLineHalfRadian)) - 1;
	float b = 2 / FMath::Sin(twoLineHalfRadian) * FVector2D::DotProduct(twoLineCenterVec, intersection - oppositePoint);
	float c = FMath::Square((intersection - oppositePoint).Length());
	
	float d = FMath::Square(b) - 4 * a * c;
	float r;
	 if (d > 0)
	 {
	 	float r1 = (-b + FMath::Sqrt(d)) / 2 / a;
	 	float r2 = (-b - FMath::Sqrt(d)) / 2 / a;
	 	r = r1 <= r2 ? r1 : r2;
	 }
	 else if (d < 0)
	 {
	 	return oppositePoint;
	 }
	 else
	 {
	 	r = -b / 2 / a;
	}
	 return intersection + twoLineCenterVec * r / FMath::Sin(twoLineHalfRadian);
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

std::pair<bool, FVector2D> ASquareStage::GetPointOfTangency(FVector2D point, FVector2D stageEdge1, FVector2D stageEdge2)
{
	std::pair result = {false, FVector2D(0.0f, 0.0f)};
	
	FVector2D stageLine = stageEdge2 - stageEdge1;
	FVector2D pointToStageEdge1 = stageEdge1 - point;
	
	//MEMO:  FVector2.DotProduct (pointToStageEdge1 + t * stageLine , stageLine) = 0 （直交する）をtに関して解く
	float t = - (pointToStageEdge1.X * stageLine.X + pointToStageEdge1.Y * stageLine.Y) / (stageLine.X * stageLine.X + stageLine.Y * stageLine.Y);
	
	if (t >= 0 && t <= 1)
	{
		result.first = true;
		result.second = stageEdge1 + t * stageLine;
	}
	return result;
}

bool ASquareStage::CompareFarPoint(std::pair<bool, FVector2D> point1, std::pair<bool, FVector2D> point2, FVector2D oppositePoint)
{
	//二辺のうち有効でないものがあった場合の処理
	if (!point1.first)
	{
		UE_LOG(LogTemp, Log, TEXT("交点のない辺がありました。: %s, point位置: %s"), *point1.second.ToString(), *oppositePoint.ToString());
		return false;//point2を有効とみなす
	}
	if (!point2.first)
	{
		UE_LOG(LogTemp, Log, TEXT("交点のない辺がありました。: %s, point位置: %s"), *point2.second.ToString(), *oppositePoint.ToString());
		return true;//point1を有効とみなす
	}
	
	//遠い方の点を返す
	if ((point1.second - oppositePoint).Length() < (point2.second - oppositePoint).Length())
	{
		return false;
	}
	return true;
}
