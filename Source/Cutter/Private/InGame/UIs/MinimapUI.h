#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapUI.generated.h"

class UOverlay;
class AInGameState;
class UImage;
class UTextBlock;


UCLASS()
class UMinimapUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/*描画する線情報を登録する*/
	void UpdateDrawLines(const TArray<FVector2D>& points, FVector cameraPos, float cameraOrthoWidth);

	/*MiniMapに線を描画するための更新用関数*/
	virtual int32 NativePaint(const FPaintArgs& Args,
							  const FGeometry& AllottedGeometry,
							  const FSlateRect& MyCullingRect,
							  FSlateWindowElementList& OutDrawElements,
							  int LayerId,
							  const FWidgetStyle& InWidgetStyle,
							  bool bParentEnabled) const override;
	
	
	void SetParentVisible(bool visible);

protected:
	UPROPERTY(EditAnywhere, Category = "UI設定")
	float _miniMapLineThickness = 2;
	
private:
	/*MiniMapに指定の点を繋いだ線を描画する*/
	void DrawCutterPath(const FPaintGeometry& geometry, FSlateWindowElementList&
						outDrawElements, int32 layerId) const;
	
private:
	TArray<FVector2D> _points = {};
	bool isParentVisible = false;
};
