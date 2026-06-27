#pragma once

#include "CannonBall.generated.h"

class IActorTransform;
class UTimelineComponent;
struct FCannonParam;

/**
 *砲丸クラス
 */
UCLASS()
class CUTTER_API ACannonBall : public AActor
{
	GENERATED_BODY()
	
public:
	ACannonBall();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	/*生成時、オブジェクトプールから持ってくるため、ここで初期化する*/
	void ReStart();
	
	/*オブジェクトプールに戻す用の関数を登録する*/
	void RegisterReleaseFunc(const TFunction<void(ACannonBall* cannonBall)>& releaseFunc);
	
	/*プレイヤーの位置情報取得用クラスを登録する*/
	void RegisterPlayerLocation(const TScriptInterface<IActorTransform>& playerTransform);
	
	/*アニメーション用のタイムラインを初期化する*/
	void InitTimeline(UStaticMeshComponent* staticMeshComponent);

	/*アクティブ時に再生されるアニメーションを実行する*/
	void PlayMoveStartAnimation();
	
	/*再生中のアニメーションがあったら終了する*/
	void ClearAllAnimation();
	
protected:
	/*アクティブ時に再生されるアニメーションが終了する際に呼ばれるコールバック*/
	virtual void OnEndMoveStartAnimation();
	
	UFUNCTION() void HandleSizeUpUpdate(float value);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Sealed")
	UStaticMeshComponent* GetMainMesh();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Sealed")
	UStaticMeshComponent* GetCollisionMesh();

protected:
	/*サイズアップアニメーション用カーブ情報*/
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TObjectPtr<UCurveFloat> _sizeUpCurve;
	
	UPROPERTY(EditAnywhere, Category = "大砲の設定")
	float moveStartAnimationDuration = 2.f;
	
	UPROPERTY(EditAnywhere, Category = "大砲の設定")
	int32 damage = 100;
	
	UPROPERTY(EditAnywhere, Category = "大砲の設定")
	FVector gravity = FVector(0, -9.81f, 0);
	
	UPROPERTY(EditAnywhere, Category = "大砲軌道の設定")
	float maxHeight = 300.f;
	
	UPROPERTY(EditAnywhere, Category = "大砲軌道の設定")
	float duration = 3.f;
	
private:
	TFunction<void(ACannonBall* cannonBall)> _releaseFunc = {};
	FTimerHandle _startAnimationTimerHandle = {};
	
	FVector initVelocity = {};
	float _currentTime = 0.f;
	
	/*サイズ変更アニメーションをかける時のための、元サイズのキャッシュ*/
	FVector _originSizeCache = {};
	
	/*c++で生成するアニメーション用Timeline。このオブジェクトの動き出し時に機能する*/
	UPROPERTY() TObjectPtr<UTimelineComponent> _moveStartAnimTimeline;
	
	/*プレイヤーの居場所を取得できる*/
	UPROPERTY() TScriptInterface<IActorTransform> _playerTransform = {};
	
	/*c++で生成したサイズアップアニメーション用マテリアルインスタンス。GC対策でキャッシュしておく*/
	UPROPERTY() TObjectPtr<UMaterialInstanceDynamic> _dynamicMaterial;
};
