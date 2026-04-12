#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CutterBase.generated.h"

struct FCutterBaseParam;
class UTimelineComponent;
class IStageShape;

UCLASS()
class CUTTER_API ACutterBase : public AActor
{
	GENERATED_BODY()
	
public:
	ACutterBase();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void ReStart(){}
	void RegisterStageShapeData(const TScriptInterface<IStageShape>& stageShape);
	using ScoreAddFunc = TFunction<void(int)>;
	void RegisterScoreAddFunc(const ScoreAddFunc& func);
	void RegisterReleaseFunc(const TFunction<void(ACutterBase* cutter)>& releaseFunc);
	
protected:
	/*このオブジェクトの可変パラメータ。override必須*/
	virtual FCutterBaseParam* GetParam(){ return nullptr; }
	using OverlapFunc = TFunction<void(AActor*)>;
	void RegisterStaticMeshEvent(UStaticMeshComponent* staticMeshComponent, const OverlapFunc& func);
	void InitTimeline(UStaticMeshComponent* staticMeshComponent);
	void PlayMoveStartAnimation();
	
	/*自身が投げられた直後に呼ばれる。演出などを実装する用*/
	UFUNCTION(BlueprintCallable, Category = "Cutter")
	void OnThrown(){}


protected:
	/*c++で生成した点滅アニメーション用Timeline。GC対策でキャッシュしておく*/
	UPROPERTY()
	TObjectPtr<UTimelineComponent> _moveStartAnimTimeline;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> _dynamicMaterial;
	
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TObjectPtr<UCurveFloat> _sizeUpCurve;

	OverlapFunc _overlapFunc = {};
	ScoreAddFunc _scoreAddFunc = {};
	TFunction<void(ACutterBase* cutter)> _releaseFunc = {};
	TScriptInterface<IStageShape> _stageShape = {};
	
private:
	UFUNCTION()
	virtual void OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void OnEndMoveStartAnimation();
	
	UFUNCTION()
	void HandleSizeUpUpdate(float value);

private:
	FTimerHandle _startAnimationTimerHandle = {};
	
	/*サイズ変更アニメーションをかける時のための、元サイズのキャッシュ*/
	FVector _originSizeCache = {};
};
