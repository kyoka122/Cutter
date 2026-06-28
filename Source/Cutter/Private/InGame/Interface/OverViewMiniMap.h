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
	
	/*MiniMapにCutterの軌道情報を描画する*/
	virtual void UpdateLine(const TArray<FVector2D>& linePoints, FVector cameraPos, float cameraOrthoWidth) = 0;
	
	/*MiniMapにアイコン情報を描画する*/
	virtual void UpdateBambooIcon(const TArray<FVector2D>& bambooIconPoints, FVector cameraPos, float cameraOrthoWidth) = 0;
};