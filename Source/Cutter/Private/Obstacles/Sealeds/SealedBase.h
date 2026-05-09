#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Utility/ObjectPool.h"
#include "SealedBase.generated.h"

class UTimelineComponent;
class ACutterBase;
struct FSealedBaseParam;

/*
 *レリックのBaseクラス
 */
UCLASS()
class CUTTER_API ASealedBase : public AActor
{
	GENERATED_BODY()

public:
	ASealedBase();
	virtual void ReStart();
	
	/*オブジェクトプールに戻す用の関数を登録する*/
	void RegisterReleaseFunc(const TFunction<void(ASealedBase* sealed)>& releaseFunc);
	
	/*カッターのSpawnerを登録する*/
	void RegisterCutterSpawner(const TSharedPtr<ObjectPool<ACutterBase>>& cutterPool);
	
	/*アニメーション用のタイムラインを初期化する*/
	void InitTimeline(UStaticMeshComponent* staticMeshComponent);
	
	/*カッターに変形する（実際はこのアクタをReleaseしてカッターアクタを別途生成する）*/
	ACutterBase* TransformCutter();
	
	/*メッシュのアルファカラーを設定する*/
	void SetMeshAlphaColor(float value) const;
	
	/*アクティブ時に再生されるアニメーションを実行する*/
	void PlayMoveStartAnimation();
	
	/*時間経過で消える前に再生されるアニメーションを実行する*/
	void PlayMoveEndAnimation();

	/*このオブジェクトの可変パラメータ。override必須*/
	virtual FSealedBaseParam* GetParam(){ return nullptr; }
	
	/*再生中のアニメーションがあったら終了する*/
	void ClearAllAnimation();
	
	/*他のオブジェクトとのOverlap判定用Componentを有効化or無効化する*/
	void SetEnableOverlapComponent();
	
protected:
	float _lifeTime = 0.f;
	FGameplayTag _type = {};
	TFunction<void(ASealedBase* sealed)> _releaseFunc = {};
	bool _isPlayingMoveEndAnimation = false;
	
	/*カッター生成用のプール*/
	TSharedPtr<ObjectPool<ACutterBase>> _cutterPool;
	
	/*サイズアップアニメーション用カーブ情報*/
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TObjectPtr<UCurveFloat> _sizeUpCurve;
	
	/*サイズアップアニメーション用カーブ情報*/
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TObjectPtr<UCurveFloat> _blinkCurve;
	
	bool _canCollisionOtherObject = false;
	
	/*アクティブ時に再生されるアニメーションが終了する際に呼ばれるコールバック*/
	virtual void OnEndMoveStartAnimation();
	
	/*時間経過で消える前に再生されるアニメーションが終了する際に呼ばれるコールバック*/
	virtual void OnEndMoveEndAnimation();
	
	UFUNCTION() void HandleBlinkUpdate(float value) const;
	UFUNCTION() void HandleSizeUpUpdate(float value);

private:
	FTimerHandle _startAnimationTimerHandle = {};
	FTimerHandle _endAnimationTimerHandle = {};
	
	/*サイズ変更アニメーションをかける時のための、元サイズのキャッシュ*/
	FVector _originSizeCache = {};
	
	/*c++で生成するアニメーション用Timeline。このオブジェクトの動き出し時に機能する*/
	UPROPERTY() TObjectPtr<UTimelineComponent> _moveStartAnimTimeline;
	
	/*c++で生成するアニメーション用Timeline。このオブジェクトが消える時に機能する*/
	UPROPERTY() TObjectPtr<UTimelineComponent> _moveEndAnimTimeline;
	
	/*c++で生成したサイズアップアニメーション用マテリアルインスタンス。GC対策でキャッシュしておく*/
	UPROPERTY() TObjectPtr<UMaterialInstanceDynamic> _dynamicMaterial;
};
