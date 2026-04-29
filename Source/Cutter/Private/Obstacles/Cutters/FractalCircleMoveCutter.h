#pragma once

#include "CutterBase.h"
#include "InGame/Interface/Breakable.h"
#include "InGame/Interface/Throwable.h"
#include "Struct/FractalCircleMoveCutterParam.h"
#include "FractalCircleMoveCutter.generated.h"

struct FCircleMoveThrowParam;

UCLASS()
class AFractalCircleMoveCutter : public ACutterBase, public IThrowable, public IBreakable
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	
	/*このアクタを破壊する*/
	virtual void Break() override;
	
	/*このカッターを投げる際のターゲットを行う*/
	virtual void StartTargeting_Implementation(AActor* throwActor) override;
	
	/*このアクタの位置を取得する*/
	virtual FVector GetLocation_Implementation() const override { return GetActorLocation(); }

protected:
	virtual void BeginPlay() override;
	void InstanceCutterChildren();
	void TransformCutterChildren(const FMatrix& parentMatrix, int depth, int& instanceIndex);
	virtual FCutterBaseParam* GetParam() override { return &_param; }
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Cutter")
	UInstancedStaticMeshComponent* GetIsmStaticMesh();

protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FFractalCircleMoveCutterParam _param = {};

private:
	/*このカッターを投げる*/
	void Throw(const FCircleMoveThrowParam& param);
	
	/*毎Tick呼ぶことでTransformを更新する*/
	void Translate(float deltaTime);
	
	/*一番中心のメッシュ位置を更新*/
	FVector CalcParentPosition(float deltaTime);
	FRotator CalcParentRotation(float deltaTime) const;
	
	void OnOverlapScoreTargetActor(AActor* otherActor) const;
	void OnOverlapDamageableActor(AActor* otherActor) const;
	void OnBreak();
	void ResetTransformParam(FVector2D toStageCenterVec2D);
	
private:
	/*基準値を中心に回転する際の半径*/
	float _rotateRadius = 0.f;
	
	/*現在のステージに対する回転角度*/
	float _currentRadius = 0.f;
	
	/*回転基準座標*/
	FVector _rotateCenterPos = {};
	
	/*円回転の向き。 1 or -1*/
	float _rotateDirection = 0.f;
	
	/*全子オブジェクトの総数を計算してキャッシュしておく用*/
	int _totalInstanceCount = 0;
	
	/*子オブジェクト達を大量描画する際に1回のドローコールで済むようにInstancedMeshを使う*/
	UPROPERTY() TObjectPtr<UInstancedStaticMeshComponent> _ismComponent = {};
};
