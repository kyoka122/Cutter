#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacles/Cannon/CannonBall.h"
#include "Utility/PoolObjectGenerator.h"
#include "CannonBallGenerator.generated.h"

/*
 *大砲のGenerator
 */
UCLASS(BlueprintType)
class CUTTER_API ACannonBallGenerator : public AActor, public PoolObjectGenerator<ACannonBall>
{
	GENERATED_BODY()

public:
	/*生成するPrefabを登録する*/
	void RegisterGeneratePrefab(TSubclassOf<ACannonBall> prefab);
	
	/*動作に必要な各種メンバを登録する*/
	void RegisterParam(const TFunction<void(int)>& scoreAddFunc, const TFunction<void(ACannonBall* cannonBall)>& releaseFunc,
		 const TScriptInterface<IActorTransform>& playerTransform);
	
	/*オブジェクトプールに対して新規生成を行う*/
	virtual ACannonBall* Generate() override;
	
	/*プールから使用する際の初期化処理(transform指定可)*/
	virtual void Activate(ACannonBall* cannonBall, const FTransform& transform) override;
	
	/*プールから使用する際の初期化処理*/
	virtual void Activate(ACannonBall* cannonBall) override {}
	
	/*プールに戻す際の初期化処理*/
	virtual void Deactivate(ACannonBall* cannonBall) override;
	
protected:
	/*生成するPrefab*/
	UPROPERTY() TSubclassOf<ACannonBall> _prefab;
	
	/*スコア加算用関数*/
	TFunction<void(int)> _scoreAddFunc = {};
	
	/*各オブジェクトをプールに返却する時の関数*/
	TFunction<void(ACannonBall* cannonBall)> _releaseFunc = {};
	
	/*プレイヤーの位置情報*/
	UPROPERTY() TScriptInterface<IActorTransform> _playerTransform = {};
	
};
