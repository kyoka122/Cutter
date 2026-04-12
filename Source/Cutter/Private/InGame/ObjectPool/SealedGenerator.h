#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utility/PoolObjectGenerator.h"
#include "SealedGenerator.generated.h"

class ASealedBase;

/*
 *レリックのGenerator。レリックの有効、無効化、初期化処理などを行う。
 */
UCLASS(BlueprintType)
class CUTTER_API ASealedGenerator : public AActor, public PoolObjectGenerator<ASealedBase>
{
	GENERATED_BODY()

public:
	/*生成するPrefabを登録する*/
	void RegisterGeneratePrefab(TSubclassOf<ASealedBase> prefab);
	
	/*動作に必要な各種メンバを登録する*/
	void RegisterParam(const TFunction<void(ASealedBase* sealed)>& releaseFunc);
	
	/*オブジェクトプールに対して新規生成を行う*/
	virtual ASealedBase* Generate() override;
	
	/*プールから使用する際の初期化処理(transform指定可)*/
	virtual void Activate(ASealedBase* sealed, const FTransform& transform) override;
	
	/*プールから使用する際の初期化処理*/
	virtual void Activate(ASealedBase* sealed) override;
	
	/*プールに戻す際の初期化処理*/
	virtual void Deactivate(ASealedBase* sealed) override;
	
	/*プールに戻す際の初期化処理。BeginPlayが必要ないもののみ初期化する。(主にアクタSpawn直後に使用)*/
	void SafeDeactivate(ASealedBase* sealed);

protected:
	/*生成するPrefab*/
	TSubclassOf<ASealedBase> _prefab;
	
	/*各オブジェクトをプールに返却する時のFunction*/
	TFunction<void(ASealedBase* cutter)> _releaseFunc = {};
};
