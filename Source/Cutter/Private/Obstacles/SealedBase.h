#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"

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
	
	/*
	 * 
	 */
	typedef TFunction<TObjectPtr<ACutterBase>(FGameplayTag, FTransform)> TransformFunc;
	void RegisterTransformCutterFunc(FGameplayTag type, const TransformFunc& transformFunc);
	void RegisterInactiveFunc(TFunction<void()> desteroyFunc);
	void SetMeshAlphaColor(float value);
	void InitTimeline(UStaticMeshComponent* staticMeshComponent);
	void PlayMoveStartAnimation();
	void PlayMoveEndAnimation();

protected:
	/*このオブジェクトの可変パラメータ。override必須*/
	virtual FSealedBaseParam* GetParam(){ return nullptr; }
	
protected:
	float _lifeTime = 0.f;
	FGameplayTag _type = {};
	TransformFunc _transformFunc = {};
	TFunction<void()> _destroyFunc = {};
	bool _playingMoveEndAnimation = false;
	
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
	void HandleBlinkUpdate(float value);
	
	UFUNCTION()
	void HandleSizeUpUpdate(float value);
	

private:
	FTimerHandle _startAnimationTimerHandle = {};
	FTimerHandle _endAnimationTimerHandle = {};
	
	/*サイズ変更アニメーションをかける時のための、元サイズのキャッシュ*/
	FVector _originSizeCache = {};
};
