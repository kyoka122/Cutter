#include "MinimapUI.h"

void UMinimapUI::UpdateDrawLines(const TArray<FVector2D>& points, FVector cameraPos, float cameraOrthoWidth)
{
	_points.Reset();//メモリを保持したまま要素数0に
	_points.Reserve(points.Num());
	for (const auto& point : points)
	{
		FVector2D normalizedPoint = (point - FVector2D(cameraPos)) / cameraOrthoWidth;//clippingに変換
		FVector2D widgetPoint(normalizedPoint.Y + 0.5f, -normalizedPoint.X + 0.5f);//Widgetの座標系に変換
		_points.Add(widgetPoint);
	}
}

int32 UMinimapUI::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 layerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	if (_points.Num() <= 0 || !isParentVisible)
	{
		return layerId;
	}
	DrawCutterPath(AllottedGeometry.ToPaintGeometry(), OutDrawElements, layerId + 1);
	return layerId;
}

void UMinimapUI::DrawCutterPath(const FPaintGeometry& geometry, FSlateWindowElementList& outDrawElements, int32 layerId) const
{
	FVector2D localSize = geometry.GetLocalSize();
	TArray<FVector2D> points;
	points.Reserve(_points.Num());
	for (auto point : _points)
	{
		points.Add(point * localSize);//X,Yが逆になる
	}
	
	FSlateDrawElement::MakeLines(outDrawElements,
		layerId,
		geometry,
		points,
		ESlateDrawEffect::None,
		FLinearColor::Red,
		true,
		_miniMapLineThickness);
}

void UMinimapUI::SetParentVisible(bool visible)
{
	isParentVisible = visible;
}
