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
	UE_LOG(LogTemp, Log, TEXT("--------------------------------------------"));
	FBox2D edge = FBox2D(GetEdgeMin(),GetEdgeMax());//TODO: FBox2DをFVector2Dを4つに直す
	TArray<FVector2D> centerPositions;
	
	FVector2D toRightVec = FVector2D(0, edge.Max.Y - edge.Min.Y);
	FVector2D toLeftVec = FVector2D(0, edge.Min.Y - edge.Max.Y);
	FVector2D toForwardVec = FVector2D(edge.Max.X - edge.Min.X, 0);
	FVector2D toBackVec = FVector2D(edge.Min.X - edge.Max.X, 0);
	
	FVector2D rightBackPos = FVector2D(edge.Min.X, edge.Max.Y);
	FVector2D leftBackPos = FVector2D(edge.Min.X, edge.Min.Y);
	FVector2D rightForwardPos = FVector2D(edge.Max.X, edge.Max.Y);
	FVector2D leftForwardPos = FVector2D(edge.Max.X, edge.Min.Y);
	
	//正方形であれば90°で問題ないので↓は必要ない
	//under x right
	centerPositions.Append(GetCircleCenterPos(
		FVector2D(0, edge.Min.Y - edge.Max.Y), 
		FVector2D(edge.Max.X - edge.Min.X, 0), 
		FVector2D(edge.Min.X, edge.Max.Y),
		oppositePoint));
	
	//under x right
	centerPositions.Append(GetCircleCenterPos(
		FVector2D(0, edge.Max.Y - edge.Min.Y),
		FVector2D(edge.Max.X - edge.Min.X, 0),
		FVector2D(edge.Min.X, edge.Min.Y),
		oppositePoint));
	
	//upper x right
	centerPositions.Append(GetCircleCenterPos(
		FVector2D(0, edge.Min.Y - edge.Max.Y),
		FVector2D(edge.Min.X - edge.Max.X, 0),
		FVector2D(edge.Max.X, edge.Max.Y),
		oppositePoint));
	
	//upper x left
	centerPositions.Append(GetCircleCenterPos(
		FVector2D(0, edge.Max.Y - edge.Min.Y),
		FVector2D(edge.Min.X - edge.Max.X, 0),
		FVector2D(edge.Max.X, edge.Min.Y),
		oppositePoint));
	
	
	FVector2D maxCenterPoint = {};
	float maxRadius = 0.f;
	for (FVector2D point : centerPositions)
	{
		UE_LOG(LogTemp, Log, TEXT("point: %s"), *point.ToString());
		float radius = (point - oppositePoint).Length();
		//円の半径が、円の中心と各ステージの辺までの距離より大きければ(=円がステージを飛び出ていたら) 除外する
		if (radius > GetMinDistance(point, rightBackPos, leftBackPos) ||
			radius > GetMinDistance(point, leftBackPos, leftForwardPos) ||
			radius > GetMinDistance(point, leftForwardPos, rightForwardPos) ||
			radius > GetMinDistance(point, rightForwardPos, rightBackPos))
		{
			continue;
		}
		
		UE_LOG(LogTemp, Log, TEXT("results: %s"), *point.ToString());
		if (radius > maxRadius)
		{
			maxRadius = radius;
			maxCenterPoint = point;
		}
	}
	
	return maxCenterPoint;
}

float ASquareStage::GetMaxRadius()
{
	FBox2D edge = FBox2D(GetEdgeMin(),GetEdgeMax());
	float xLength = FMath::Abs(edge.Max.X - edge.Min.X);
	float yLength = FMath::Abs(edge.Max.Y - edge.Min.Y);
	return FMath::Max(xLength/2, yLength/2);
}

TArray<FVector2D> ASquareStage::GetCircleCenterPos(FVector2D firstLineVec, FVector2D secondLineVec, FVector2D intersection, FVector2D oppositePoint)
{
	//「1. rの長さと、二辺の交点～円の中心までの関係性」と、「2. oppositePoint～円の中心までの距離の関係性」を　元に連立方程式を解く
	// oppositePointをP, 二辺の交点をQ、円の中心をOとすると...
	
	//1. rの長さと、二辺の交点～円の中心までの関係性: PO→ = r/sin(θ/2) * (firstLine.Normalize() + secondLine.Normalize()) + PQ→
	//2. oppositePoint～円の中心までの距離の関係性: |OP→| = r
	
	float twoLineHalfRadian = FMath::Acos(FVector2D::DotProduct(firstLineVec, secondLineVec) / (firstLineVec.Length() * secondLineVec.Length())) / 2;
	FVector2D twoLineCenterVec = (firstLineVec.GetSafeNormal() + secondLineVec.GetSafeNormal()).GetSafeNormal();
	
	float a = 1 / FMath::Square(FMath::Sin(twoLineHalfRadian)) - 1;
	float b = 2 / FMath::Sin(twoLineHalfRadian) * FVector2D::DotProduct(twoLineCenterVec, intersection - oppositePoint);
	float c = FMath::Square((intersection - oppositePoint).Length());
	
	float d = FMath::Square(b) - 4 * a * c;
	TArray<FVector2D> results;
	
	if (d > 0)
	{
		float r1 = (-b + FMath::Sqrt(d)) / 2 / a;
		float r2 = (-b - FMath::Sqrt(d)) / 2 / a;
		UE_LOG(LogTemp, Log, TEXT("r1: %f"), r1);
		UE_LOG(LogTemp, Log, TEXT(" %s"), *(intersection + twoLineCenterVec * r1 / FMath::Sin(twoLineHalfRadian)).ToString());
		UE_LOG(LogTemp, Log, TEXT("r2: %f"), r2);
		UE_LOG(LogTemp, Log, TEXT(" %s"), *(intersection + twoLineCenterVec * r2 / FMath::Sin(twoLineHalfRadian)).ToString());
		results.Add(intersection + twoLineCenterVec * r1 / FMath::Sin(twoLineHalfRadian));
		results.Add(intersection + twoLineCenterVec * r2 / FMath::Sin(twoLineHalfRadian));
	}
	else if (d < 0)
	{
		results.Add(oppositePoint);
	}
	else
	{
		float r = -b / 2 / a;
		results.Add(intersection + twoLineCenterVec * r / FMath::Sin(twoLineHalfRadian));
	}
	return results;
}

float ASquareStage::GetMinDistance(FVector2D point, FVector2D stageEdge1, FVector2D stageEdge2)
{
	float cross = FVector2D::CrossProduct(point - stageEdge1, stageEdge2 - stageEdge1);
	
	if (FMath::IsNearlyZero(cross))
	{
		return 0;//直線の上に点がある場合
	}
	
	UE_LOG(LogTemp, Log, TEXT("  distance: %f"),  FMath::Abs(cross)/(stageEdge2 - stageEdge1).Length());
	return FMath::Abs(cross)/(stageEdge2 - stageEdge1).Length();
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
