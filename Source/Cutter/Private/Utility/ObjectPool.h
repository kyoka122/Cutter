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
	ObjectPool(PoolObjectGenerator<T>* generator);
	
	/*Poolから取得する(transform指定有)*/
	TObjectPtr<T> Create(const FTransform& transform);
	
	/*Poolから取得する*/
	TObjectPtr<T> Create();
	
	/*Poolから現在使用中のCutterを全て取得する*/
	TArray<T*> GetCurrentUsingObject() const;
	
	/*Poolにオブジェクトを戻す*/
	void Release(TObjectPtr<T> object);
	
private:
	/*現在使用されていないオブジェクトを探す*/
	ObjectPoolManageData* GetNotUsingObjectData();
	
private:
	TArray<TSharedPtr<ObjectPoolManageData>> _poolData = {};
	PoolObjectGenerator<T>* _generator = {};//TODO: デストラクタでdeleteするとクラッシュするので一旦このまま生ポインタ
};