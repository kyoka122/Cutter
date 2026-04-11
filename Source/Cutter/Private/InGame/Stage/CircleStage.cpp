#include "CircleStage.h"

bool ACircleStage::IsInStage_Implementation(FVector2D point)
{
	float stageCenterToPointDistance = (point - FVector2D(Execute_GetCenterPos(this))).Length();
	return GetRadius() >= stageCenterToPointDistance;
}

FIntersectionData ACircleStage::GetInterSection_Implementation(FVector2D viaPoint, FVector2D vec)
{
	FIntersectionData intersectionData;
	FVector2D centerPos2D = FVector2D(Execute_GetCenterPos(this));
	//MEMO:
	//a+tb = p, |p-c| = rの連立方程式  |a-c+tb| = r の解を求める
	// => viaPoint + t*vec = d, |d-c| = GetRadius()の連立方程式  |viaPoint - GetCenterPos() + t*vec| = GetRadius() の解を求める
	float a = FMath::Square(vec.Length());
	float b = - 2 * FVector2D::DotProduct(viaPoint - centerPos2D,vec);
	float c = FMath::Square((viaPoint - centerPos2D).Length()) - FMath::Square(GetRadius());
	
	float d = FMath::Square(b) - 4 * a * c;
	if (d > 0)
	{
		float t1 = (-b + FMath::Sqrt(d)) / 2 / a;
		float t2 = (-b - FMath::Sqrt(d)) / 2 / a;
		intersectionData.isIn = true;
		intersectionData.isTangent = false;
		intersectionData.point1 = viaPoint + t1 * vec;
		intersectionData.point2 = viaPoint + t2 * vec;
	}
	else if (d < 0)
	{
		intersectionData.isIn = false;
	}
	else
	{
		float t1 = -b / 2 / a;
		intersectionData.isIn = true;
		intersectionData.isTangent = true;
		intersectionData.point1 = viaPoint + t1 * vec;
		intersectionData.point2 = viaPoint + t1 * vec;
	}
	return intersectionData;
}

FVector2D ACircleStage::GetPointOfTangency_Implementation(FVector2D oppositePoint)
{
	FVector2D oppositePointToStageCenterVec2D = FVector2D(Execute_GetCenterPos(this)) - oppositePoint;
	float oppositePointToStageCenterDistance = oppositePointToStageCenterVec2D.Length();
	oppositePointToStageCenterVec2D.Normalize();
	float oppositePointToPointOfTangencyDistance = GetRadius() + oppositePointToStageCenterDistance;//指定座標から接点までの距離
	return oppositePoint + oppositePointToStageCenterVec2D * oppositePointToPointOfTangencyDistance;//指定座標 + 方向ベクトル * 接点までの距離
}
