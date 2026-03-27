#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
template <typename T = UObject>
class CUTTER_API PoolObjectGenerator
{
	static_assert(TIsDerivedFrom<T,UObject>::Value, "ObjectPool must be derived from UObject");
	
public:
	virtual ~PoolObjectGenerator() = default;
	
	virtual TObjectPtr<T> Generate() = 0;
	virtual void Activate(TObjectPtr<T> obj, FTransform transform) = 0;
	virtual void Activate(TObjectPtr<T> obj) = 0;
	virtual void Deactivate(TObjectPtr<T> obj) = 0;
};