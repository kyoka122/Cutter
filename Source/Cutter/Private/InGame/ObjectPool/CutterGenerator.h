#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utility/PoolObjectGenerator.h"
#include "CutterGenerator.generated.h"

class IStageShape;
class ACutterBase;

/*
 *カッターのGenerator。レリックの有効、無効化、初期化処理などを行う。
 */
UCLASS(BlueprintType)
class CUTTER_API ACutterGenerator : public AActor, public PoolObjectGenerator<ACutterBase>
{
	GENERATED_BODY()

public:
	/*生成するPrefabを登録する*/
	void RegisterGeneratePrefab(TSubclassOf<ACutterBase> prefab);
	
	/*動作に必要な各種メンバを登録する*/
	void RegisterParam(const TFunction<void(int)>& scoreAddFunc, const TFunction<void(ACutterBase* cutter)>& releaseFunc,
		const TScriptInterface<IStageShape>& stageShape);
	
	/*オブジェクトプールに対して新規生成を行う*/
	virtual ACutterBase* Generate() override;
	
	/*プールから使用する際の初期化処理(transform指定可)*/
	virtual void Activate(ACutterBase* cutter, const FTransform& transform) override;
	
	/*プールから使用する際の初期化処理*/
	virtual void Activate(ACutterBase* cutter) override {}
	
	/*プールに戻す際の初期化処理*/
	virtual void Deactivate(ACutterBase* cutter) override;
	
protected:
	/*生成するPrefab*/
	TSubclassOf<ACutterBase> _prefab;
	
	/*スコア加算用関数*/
	TFunction<void(int)> _scoreAddFunc = {};
	
	/*各オブジェクトをプールに返却する時の関数*/
	TFunction<void(ACutterBase* cutter)> _releaseFunc = {};
	
	/*ステージの地形情報*/
	TScriptInterface<IStageShape> _stageShape = {};
};
