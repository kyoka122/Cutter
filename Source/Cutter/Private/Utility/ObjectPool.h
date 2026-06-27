#pragma once

#include "PoolObjectGenerator.h"

/*
 * オブジェクトプール
 */
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
	ObjectPool(PoolObjectGenerator<T>* generator)
	{
		_generator = generator;
	}

	/*Poolから取得する(transform指定有)*/
	TObjectPtr<T> Create(const FTransform& transform)
	{
		ObjectPoolManageData* param = GetNotUsingObjectData();
		param->isUsing = true;
		_generator->Activate(param->obj, transform);
		return param->obj;
	}

	/*Poolから取得する*/
	TObjectPtr<T> Create()
	{
		ObjectPoolManageData* param = GetNotUsingObjectData();
		param->isUsing = true;
		_generator->Activate(param->obj);
		return param->obj;
	}
	
	/*Poolから現在使用中のCutterを全て取得する*/
	TArray<T*> GetCurrentUsingObject() const
	{
		TArray<T*> usingObjects;
		usingObjects.Reserve(_poolData.Num());//メモリ確保
		for (auto& poolData : _poolData)
		{
			if (poolData.IsValid() && poolData->isUsing)
			{
				usingObjects.Add(poolData.Get()->obj);
			}
		}
		return usingObjects;
	}
	
	/*Poolにオブジェクトを戻す*/
	void Release(TObjectPtr<T> object)
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
		UE_LOG(LogTemp, Error, TEXT("ObjectPoolのReleaseが完了できませんでした: %s"), *object.GetName());
	}
	
private:
	/*現在使用されていないオブジェクトを探す*/
	ObjectPoolManageData* GetNotUsingObjectData()
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
	
private:
	TArray<TSharedPtr<ObjectPoolManageData>> _poolData = {};
	PoolObjectGenerator<T>* _generator = {};//TODO: デストラクタでdeleteするとクラッシュするので一旦このまま生ポインタ
};