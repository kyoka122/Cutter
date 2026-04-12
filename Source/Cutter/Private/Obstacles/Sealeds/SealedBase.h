#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Utility/ObjectPool.h"
#include "SealedBase.generated.h"

class UTimelineComponent;
class ACutterBase;
struct FSealedBaseParam;

UCLASS()
class CUTTER_API ASealedBase : public AActor
{
	GENERATED_BODY()

public:
	ASealedBase();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void ReStart(){}
	void RegisterReleaseFunc(const TFunction<void(ASealedBase* sealed)>& releaseFunc);
	void RegisterSpawner(const TSharedPtr<ObjectPool<ACutterBase>>& cutterPool);
	void InitTimeline(UStaticMeshComponent* staticMeshComponent);
	ACutterBase* TransformCutter();
	void SetMeshAlphaColor(float value) const;
	void PlayMoveStartAnimation();
	void PlayMoveEndAnimation();

protected:
	/*このオブジェクトの可変パラメータ。override必須*/
	virtual FSealedBaseParam* GetParam(){ return nullptr; }
	
protected:
	float _lifeTime = 0.f;
	FGameplayTag _type = {};
	TFunction<void(ASealedBase* sealed)> _releaseFunc = {};
	bool _playingMoveEndAnimation = false;
	TSharedPtr<ObjectPool<ACutterBase>> _cutterPool;
	
	/*c++で生成するアニメーション用Timeline。このオブジェクトの動き出し時に機能する*/
	UPROPERTY()
	TObjectPtr<UTimelineComponent> _moveStartAnimTimeline;
	
	/*c++で生成する点滅アニメーション用Timeline。このオブジェクトが消える時に機能する*/
	UPROPERTY()
	TObjectPtr<UTimelineComponent> _moveEndAnimTimeline;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> _dynamicMaterial;
	
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TObjectPtr<UCurveFloat> _sizeUpCurve;
	
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TObjectPtr<UCurveFloat> _blinkCurve;

private:
	void OnEndMoveEndAnimation();
	void OnEndMoveStartAnimation();
	
	UFUNCTION()
	void HandleBlinkUpdate(float value) const;
	
	UFUNCTION()
	void HandleSizeUpUpdate(float value);
	

private:
	FTimerHandle _startAnimationTimerHandle = {};
	FTimerHandle _endAnimationTimerHandle = {};
	
	/*サイズ変更アニメーションをかける時のための、元サイズのキャッシュ*/
	FVector _originSizeCache = {};
};
