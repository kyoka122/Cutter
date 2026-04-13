#pragma once

#include "CoreMinimal.h"
#include "Obstacles/Cutters/CutterBase.h"
#include "InGame/Interface/Breakable.h"
#include "Struct/CircleExpandCutterParam.h"
#include "CircleExpandCutter.generated.h"

/**
 * 円形ステージを円を描くように回りながら、中心から徐々に外に向かって円が広がっていくカッタークラス
 */
UCLASS()
class CUTTER_API ACircleExpandCutter : public ACutterBase, public IBreakable
{
	GENERATED_BODY()

public:
	ACircleExpandCutter(){}
	virtual void Tick(float DeltaTime) override;
	
	/*このアクタを破壊する*/
	virtual void Break() override;
	
	/*生成時、オブジェクトプールから持ってくるため、ここで初期化する*/
	virtual void ReStart() override;

protected:
	virtual void BeginPlay() override;
	virtual FCutterBaseParam* GetParam() override { return &_param; }
	
protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FCircleExpandCutterParam _param;

private:
	/*毎Tick呼ぶことでTransformを更新する*/
	void Translate(float deltaTime);
	
	void CheckOutOfStage();
	FVector CalcPosition(float deltaTime);
	FQuat CalcRotation(float deltaTime) const;
	void OnOverlapBreakableActor(AActor* otherActor);
	void OnOverlapScoreTargetActor(AActor* otherActor) const;
	void OnOverlapDamageableActor(AActor* otherActor) const;
	void OnBreak();
	
private:
	/*現在のステージに対する回転角度*/
	float _currentAngle = 0.0f;
	
	/*回転基準座標*/
	FVector _rotateCenterPos = {};
};
