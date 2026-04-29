#pragma once

#include "CoreMinimal.h"
#include "Obstacles/Cutters/CutterBase.h"
#include "InGame/Interface/Breakable.h"
#include "InGame/Interface/Throwable.h"
#include "Obstacles/CharacterTargetComponents/Struct/CircleMoveCutterThrowParam.h"
#include "Struct/CircleMoveCutterParam.h"
#include "CircleMoveCutter.generated.h"

struct FStraightYoYoThrowParam;
/**
 * 円形ステージを円周上に周るカッタークラス
 */
UCLASS()
class CUTTER_API ACircleMoveCutter : public ACutterBase, public IThrowable, public IBreakable
{
	GENERATED_BODY()

public:
	ACircleMoveCutter(){}
	virtual void Tick(float DeltaTime) override;
	
	/*このアクタを破壊する*/
	virtual void Break() override;
	
	/*このカッターを投げる際のターゲットを行う*/
	virtual void StartTargeting_Implementation(AActor* throwActor) override;
	
	/*このアクタの位置を取得する*/
	virtual FVector GetLocation_Implementation() const override { return GetActorLocation(); };

protected:
	virtual void BeginPlay() override;
	virtual FCutterBaseParam* GetParam() override { return &_param; }
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Cutter")
	UStaticMeshComponent* GetStaticMesh();
	
protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FCircleMoveCutterParam _param = {};

private:
	/*このカッターを投げる*/
	void Throw(const FCircleMoveCutterThrowParam& param);
	
	/*毎Tick呼ぶことでTransformを更新する*/
	void Translate(float deltaTime);
	
	FVector CalcPosition(float deltaTime);
	FRotator CalcRotation(float deltaTime) const;
	void OnOverlapBreakableActor(AActor* otherActor);
	void OnOverlapScoreTargetActor(AActor* otherActor) const;
	void OnOverlapDamageableActor(AActor* otherActor) const;
	void OnBreak();
	void ResetTransformParam(FVector2D toStageCenterVec2D);
	
private:
	/*基準値を中心に回転する際の半径*/
	float _rotateRadius = 0.f;
	
	/*現在のステージに対する回転角度*/
	float _currentRadius = 0.f;
	
	/*円回転の向き。 1 or -1*/
	float _rotateDirection = 0.f;
	
	/*回転基準座標*/
	FVector _rotateCenterPos = {};
	
	UPROPERTY() TObjectPtr<UStaticMeshComponent> _staticMeshComponent = {};
};