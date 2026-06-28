#include "MinimapUI.h"

void UMinimapUI::UpdateDrawLines(const TArray<FVector2D>& points, FVector cameraPos, float cameraOrthoWidth)
{
	_cutterLinePoints.Reset();//メモリを保持したまま要素数0に
	_cutterLinePoints.Reserve(points.Num());
	for (const auto& point : points)
	{
		FVector2D normalizedPoint = (point - FVector2D(cameraPos)) / cameraOrthoWidth;//clippingに変換
		FVector2D widgetPoint(normalizedPoint.Y + 0.5f, -normalizedPoint.X + 0.5f);//Widgetの座標系に変換
		_cutterLinePoints.Add(widgetPoint);
	}
}

void UMinimapUI::UpdateBambooPoints(const TArray<FVector2D>& points, FVector cameraPos, float cameraOrthoWidth)
{
	_bambooPoints.Reset();//メモリを保持したまま要素数0に
	_bambooPoints.Reserve(points.Num());
	for (const auto& point : points)
	{
		FVector2D normalizedPoint = (point - FVector2D(cameraPos)) / cameraOrthoWidth;//clippingに変換
		FVector2D widgetPoint(normalizedPoint.Y + 0.5f, -normalizedPoint.X + 0.5f);//Widgetの座標系に変換
		_bambooPoints.Add(widgetPoint);
	}
}

int32 UMinimapUI::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
                              FSlateWindowElementList& OutDrawElements, int LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 layerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	if (_cutterLinePoints.Num() <= 0 || !isParentVisible)
	{
		return layerId;
	}
	DrawBambooImage(AllottedGeometry, OutDrawElements, layerId + 1);
	DrawCutterPath(AllottedGeometry.ToPaintGeometry(), OutDrawElements, layerId + 2);
	return layerId;
}

void UMinimapUI::DrawCutterPath(const FPaintGeometry& geometry, FSlateWindowElementList& outDrawElements, int32 layerId) const
{
	FVector2D localSize = geometry.GetLocalSize();
	TArray<FVector2D> points;
	points.Reserve(_cutterLinePoints.Num());
	for (auto point : _cutterLinePoints)
	{
		points.Add(point * localSize);
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

void UMinimapUI::DrawBambooImage(const FGeometry& geometry, FSlateWindowElementList& outDrawElements, int32 layerId) const
{
	FSlateBrush iconBrush;
	iconBrush.SetResourceObject(_bambooIcon);
	iconBrush.ImageSize = _bambooIconSize;
	iconBrush.ImageType = ESlateBrushImageType::Linear;
	FVector2D localSize = geometry.GetLocalSize();
	
	for (auto point : _bambooPoints)
	{
		point *= localSize;
		FSlateDrawElement::MakeBox(outDrawElements,
		layerId,
		geometry.ToPaintGeometry(iconBrush.ImageSize, FSlateLayoutTransform(point)),
		&iconBrush,
		ESlateDrawEffect::None);
	}
	
}

void UMinimapUI::SetParentVisible(bool visible)
{
	isParentVisible = visible;
}