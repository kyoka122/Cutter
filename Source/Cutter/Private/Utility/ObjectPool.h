#pragma once

#include "PoolObjectGenerator.h"

template <typename T = UObject>
class CUTTER_API ObjectPool
{
	static_assert(TIsDerivedFrom<T,UObject>::Value, "ObjectPool must be derived from UObject");
	
	class ObjectPoolManageData
	{
	public:
		ObjectPoolManageData(TObjectPtr<T> obj)
		{
			this->obj = obj;
		}
	
	public:
		bool isUsing = false;
		TObjectPtr<T> obj = {};
	};
	
public:
	ObjectPool(PoolObjectGenerator<T>* generator);
	TObjectPtr<T> Create(const FTransform& transform);
	TObjectPtr<T> Create();
	void Release(TObjectPtr<T> object);
	
private:
	ObjectPoolManageData* GetNotUsingObjectData();
	
private:
	TArray<TSharedPtr<ObjectPoolManageData>> _poolData = {};
	PoolObjectGenerator<T>* _generator = {};
};