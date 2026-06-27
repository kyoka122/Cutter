#pragma once

#include "Struct/CannonParam.h"
#include "Utility/ObjectPool.h"
#include "Cannon.generated.h"

class IActorTransform;
class ACannonBall;
class UTimelineComponent;
struct FCannonParam;

/**
 *大砲クラス
 */
UCLASS()
class CUTTER_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:
	ACannon();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	/*生成時、オブジェクトプールから持ってくるため、ここで初期化する*/
	void ReStart();
	
	/*オブジェクトプールに戻す用の関数を登録する*/
	void RegisterReleaseFunc(const TFunction<void(ACannon* cannon)>& releaseFunc);

	/*CannonBallのSpawnerを登録する*/
	void RegisterCannonBallSpawner(const TSharedPtr<ObjectPool<ACannonBall>>& cannonBallPool);
	
	/*プレイヤーの位置情報取得用クラスを登録する*/
	void RegisterPlayerLocation(const TScriptInterface<IActorTransform>& playerTransform);
	
protected:
	/*アクティブ時に再生されるアニメーションの再生時間*/
	UPROPERTY(EditAnywhere, Category = "砲台設定")
	float moveStartAnimationDuration = 2;
	
	UPROPERTY(EditAnywhere, Category = "砲台設定")
	float targetTime = 10;
	
	/*カッター生成用のプール*/
	TSharedPtr<ObjectPool<ACannonBall>> _cannonBallPool;
	
	TFunction<void(ACannon* cannon)> _releaseFunc = {};

private:
	/*砲弾に変形する（実際はこのアクタをReleaseして砲弾アクタを別途生成する）*/
	void SpawnCannonBall();
	
	/*砲台と砲弾を解放する*/
	void Release(ACannonBall* cannonBall);

private:
	float _targetTime = 0.f;
	
	/*プレイヤーの居場所を取得できる*/
	UPROPERTY() TScriptInterface<IActorTransform> _playerTransform = {};
};
