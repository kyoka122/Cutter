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
	for (auto& poolData : _poolData)
	{
		if (poolData.IsValid() && poolData->obj == object)
		{
			poolData->isUsing = false;
			return;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("ObjectPoolのReleaseが完了できませんでした: %s"), *object.GetName());
}

template <typename T>
ObjectPool<T>::ObjectPoolManageData* ObjectPool<T>::GetNotUsingObjectData()
{
	for (auto& poolData : _poolData)
	{
		if (poolData.IsValid() && !poolData->isUsing)
		{
			return poolData.Get();
		}
	}
	TObjectPtr<T> instance = _generator->Generate();
	int index = _poolData.Add(MakeShared<ObjectPoolManageData>(instance));
	return _poolData[index].Get();
}