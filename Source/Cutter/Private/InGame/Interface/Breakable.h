#pragma once

#include "CoreMinimal.h"
#include "Breakable.generated.h"

/**
 * 衝突による破壊が可能なインターフェース(UObject)
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UBreakable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 衝突による破壊が可能なインターフェース
 */
class CUTTER_API IBreakable
{
	GENERATED_BODY()
	
public:
	/*アクタを破壊する*/
	virtual void Break() = 0;
};