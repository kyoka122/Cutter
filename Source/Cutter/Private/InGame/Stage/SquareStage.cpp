#include "SquareStage.h"

ASquareStage::ASquareStage()
{
	stageEdge = FBox2D(GetEdgeMin(),GetEdgeMax());
}

bool ASquareStage::IsInStage_Implementation(FVector2D point)
{
	return stageEdge.IsInsideOrOn(point);
}

FIntersectionData ASquareStage::GetInterSection_Implementation(FVector2D point, FVector2D vec)
{
	FIntersectionData intersectionData;
	check(nullptr)
	//TODO: 外判定でだめなら即座にfalseを返す
	//TODO: 4本の線に対して交点を出して交わったものを
	return intersectionData;
}

FVector2D ASquareStage::GetPointOfTangency_Implementation(FVector2D oppositePoint)
{
	//stageEdge.Ge
	FVector2D point1 = stageEdge.Min - FVector2D(stageEdge.Min.X, stageEdge.Max.Y);
	//TODO: 4本の線に対して接点を求めて、一番長いものの接点を返す
	FVector2D oppositePointToStageCenterVec2D = oppositePoint - FVector2D(Execute_GetCenterPos(this));
	float oppositePointToStageCenterDistance = oppositePointToStageCenterVec2D.Length();
	oppositePointToStageCenterVec2D.Normalize();
	float radius = stageEdge.GetSize().X/2;
	return oppositePointToStageCenterVec2D * (radius + oppositePointToStageCenterDistance);
}
