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
	TObjectPtr<T> Get(FTransform transform);
	TObjectPtr<T> Get();
	void Release(TObjectPtr<T> object);
	
private:
	ObjectPoolManageData* GetNotUsingObjectData();
	
private:
	TArray<ObjectPoolManageData*> _poolData = {};
	PoolObjectGenerator<T>* _generator = {};
};