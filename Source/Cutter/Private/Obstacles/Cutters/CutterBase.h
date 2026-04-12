#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CutterBase.generated.h"

struct FCutterBaseParam;
class UTimelineComponent;
class IStageShape;

/*
 * CutterのBaseクラス
 */
UCLASS()
class CUTTER_API ACutterBase : public AActor
{
	GENERATED_BODY()
	
public:
	ACutterBase();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	/*生成時、オブジェクトプールから持ってくるため、ここで初期化する*/
	virtual void ReStart(){}
	
	/*今実行中のステージ形状情報を登録する*/
	void RegisterStageShapeData(const TScriptInterface<IStageShape>& stageShape);
	
	/*スコア加算用関数を登録する*/
	using ScoreAddFunc = TFunction<void(int)>;
	void RegisterScoreAddFunc(const ScoreAddFunc& func);
	
	/*オブジェクトプールに戻す用の関数を登録する*/
	void RegisterReleaseFunc(const TFunction<void(ACutterBase* cutter)>& releaseFunc);
	
protected:
	/*このオブジェクトの可変パラメータ。override必須*/
	virtual FCutterBaseParam* GetParam(){ return nullptr; }
	
	using OverlapFunc = TFunction<void(AActor*)>;
	/*衝突時のイベントを登録する*/
	void RegisterStaticMeshEvent(UStaticMeshComponent* staticMeshComponent, const OverlapFunc& func);
	
	/*アニメーション用のタイムラインを初期化する*/
	void InitTimeline(UStaticMeshComponent* staticMeshComponent);
	
	/*アクティブ時に再生されるアニメーションを実行する*/
	void PlayMoveStartAnimation();
	
	/*自身が投げられた直後に呼ばれる。演出などを実装する用*/
	UFUNCTION(BlueprintCallable, Category = "Cutter")
	void OnThrown(){}


protected:
	OverlapFunc _overlapFunc = {};
	ScoreAddFunc _scoreAddFunc = {};
	TFunction<void(ACutterBase* cutter)> _releaseFunc = {};
	TScriptInterface<IStageShape> _stageShape = {};
	
	/*サイズアップアニメーション用カーブ情報*/
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TObjectPtr<UCurveFloat> _sizeUpCurve;
	
private:
	UFUNCTION() virtual void OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/*アクティブ時に再生されるアニメーションが終了する際に呼ばれるコールバック*/
	void OnEndMoveStartAnimation();
	
	/*サイズを大きくするアニメーションの更新用関数*/
	UFUNCTION() void HandleSizeUpUpdate(float value);

private:
	FTimerHandle _startAnimationTimerHandle = {};
	
	/*c++で生成したサイズアップアニメーション用Timeline。*/
	UPROPERTY() TObjectPtr<UTimelineComponent> _moveStartAnimTimeline;
	
	/*c++で生成したサイズアップアニメーション用マテリアルインスタンス。GC対策でキャッシュしておく。*/
	UPROPERTY() TObjectPtr<UMaterialInstanceDynamic> _dynamicMaterial;
	
	/*サイズ変更アニメーションをかける時のための、元サイズのキャッシュ*/
	FVector _originSizeCache = {};
};
