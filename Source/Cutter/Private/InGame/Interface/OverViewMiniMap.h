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
};