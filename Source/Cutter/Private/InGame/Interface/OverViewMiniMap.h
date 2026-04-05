#pragma once

#include "OverViewMiniMap.generated.h"

UINTERFACE(MinimalAPI,Blueprintable)
class UOverViewMiniMap : public UInterface
{
	GENERATED_BODY()
};


class CUTTER_API IOverViewMiniMap
{
	GENERATED_BODY()
	
public:
	virtual void SetVisibilityMiniMap(bool value) = 0;
};