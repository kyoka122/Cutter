#include "ObjectPool.h"

template <typename T>
ObjectPool<T>::ObjectPool(PoolObjectGenerator<T>* generator)
{
	_generator = generator;
}

template <typename T>
TObjectPtr<T> ObjectPool<T>::Get(FTransform transform)
{
	ObjectPoolManageData* param = GetNotUsingObjectData();
	param->isUsing = true;
	_generator->Activate(param->obj, transform);
	return param->obj;
}

template <typename T>
TObjectPtr<T> ObjectPool<T>::Get()
{
	ObjectPoolManageData* param = GetNotUsingObjectData();
	param->isUsing = true;
	_generator->Activate(param->obj);
	return param->obj;
}

template <typename T>
void ObjectPool<T>::Release(TObjectPtr<T> object)
{
	_generator->Deactivate(object);
	ObjectPoolManageData** param = _poolData.FindByPredicate([object](ObjectPoolManageData* param)
	{
		return param->obj == object;
	});
	(*param)->isUsing = false;
}

template <typename T>
ObjectPool<T>::ObjectPoolManageData* ObjectPool<T>::GetNotUsingObjectData()
{
	for (auto& poolData : _poolData)
	{
		if (!poolData->isUsing)
		{
			return poolData;
		}
	}
	TObjectPtr<T> instance = _generator->Generate();
	ObjectPoolManageData* data = new ObjectPoolManageData(instance);
	_poolData.Add(data);
	return data;
}