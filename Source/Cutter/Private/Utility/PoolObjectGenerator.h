#pragma once

#include "CoreMinimal.h"

/**
 * オブジェクトのGeneratorテンプレート。レリックの有効、無効化、初期化処理などを行う
 */
template <typename T = UObject>
class CUTTER_API PoolObjectGenerator
{
	static_assert(TIsDerivedFrom<T,UObject>::Value, "ObjectPool must be derived from UObject");
	
public:
	virtual ~PoolObjectGenerator() = default;
	
	/*オブジェクトプールに対して新規生成を行う*/
	virtual T* Generate() = 0;
	
	/*プールから使用する際の初期化処理(transform指定可)*/
	virtual void Activate(T* obj, const FTransform& transform) = 0;
	
	/*プールから使用する際の初期化処理*/
	virtual void Activate(T* obj) = 0;
	
	/*プールに戻す際の初期化処理*/
	virtual void Deactivate(T* obj) = 0;
};