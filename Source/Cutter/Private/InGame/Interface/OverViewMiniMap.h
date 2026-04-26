#pragma once

#include "OverViewMiniMap.generated.h"

/*
 * 垂直見下ろし視点のMiniMapの利用インターフェース(UObject)
 */
UINTERFACE(MinimalAPI,Blueprintable)
class UOverViewMiniMap : public UInterface
{
	GENERATED_BODY()
};

/*
 * 垂直見下ろし視点のMiniMapの利用インターフェース
 */
class CUTTER_API IOverViewMiniMap
{
	GENERATED_BODY()
	
public:
	/*MiniMapの表示非表示を切り替える*/
	virtual void SetVisibilityMiniMap(bool value) = 0;
	
	/*MiniMapに指定の線を描画する*/
	virtual void UpdateDrawLines(const TArray<FVector2D>& points, FVector cameraPos, float cameraOrthoWidth) = 0;
};