#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacles/Cannon/Cannon.h"
#include "Utility/PoolObjectGenerator.h"
#include "CannonGenerator.generated.h"

/*
 *大砲のGenerator
 */
UCLASS(BlueprintType)
class CUTTER_API ACannonGenerator : public AActor, public PoolObjectGenerator<ACannon>
{
	GENERATED_BODY()

public:
	/*生成するPrefabを登録する*/
	void RegisterGeneratePrefab(TSubclassOf<ACannon> prefab);
	
	/*動作に必要な各種メンバを登録する*/
	void RegisterParam(const TFunction<void(int)>& scoreAddFunc, const TFunction<void(ACannon* cannon)>& releaseFunc);
	
	/*オブジェクトプールに対して新規生成を行う*/
	virtual ACannon* Generate() override;
	
	/*プールから使用する際の初期化処理(transform指定可)*/
	virtual void Activate(ACannon* cannon, const FTransform& transform) override;
	
	/*プールから使用する際の初期化処理*/
	virtual void Activate(ACannon* cannon) override {}
	
	/*プールに戻す際の初期化処理*/
	virtual void Deactivate(ACannon* cannon) override;
	
protected:
	/*生成するPrefab*/
	UPROPERTY() TSubclassOf<ACannon> _prefab;
	
	/*スコア加算用関数*/
	TFunction<void(int)> _scoreAddFunc = {};
	
	/*各オブジェクトをプールに返却する時の関数*/
	TFunction<void(ACannon* cannon)> _releaseFunc = {};
};
