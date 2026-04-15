#pragma once

#include "CoreMinimal.h"
#include "CutterBase.h"
#include "GameFramework/Actor.h"
#include "InGame/Interface/Breakable.h"
#include "InGame/Interface/Throwable.h"
#include "Struct/StraightYoYoCutterParam.h"
#include "StraightYoYoCutter.generated.h"

/**
 * ばね運動をするように行ったり来たりするカッタークラス
 */
UCLASS()
class CUTTER_API AStraightYoYoCutter : public ACutterBase, public IThrowable, public IBreakable
{
	GENERATED_BODY()

public:
	AStraightYoYoCutter(){}
	virtual void Tick(float DeltaTime) override;
	
	/*このアクタを破壊する*/
	virtual void Break() override;
	
	/*このカッターを投げる際のターゲットを行う*/
	virtual void StartTargeting_Implementation(AActor* throwActor) override;
	
	/*このカッターを投げる*/
	virtual void Throw_Implementation() override;
	
	/*このアクタの位置を取得する*/
	virtual FVector GetLocation_Implementation() const override { return GetActorLocation(); };

protected:
	virtual void BeginPlay() override;
	virtual FCutterBaseParam* GetParam() override { return &_param; }
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Cutter")
	UStaticMeshComponent* GetStaticMesh();
	
protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FStraightYoYoCutterParam _param = {};
	
private:
	/*毎Tick呼ぶことでTransformを更新する*/
	void Translate(float deltaTime);
	FVector CalcPosition(float deltaTime);
	FRotator CalcRotation(float deltaTime) const;
	void OnOverlapBreakableActor(AActor* otherActor);
	void OnOverlapScoreTargetActor(AActor* otherActor) const;
	void OnOverlapDamageableActor(AActor* otherActor) const;
	void SetThrowTargetParam();
	void OnBreak();

private:
	/*ばね運動の振動の中心点*/
	FVector2D _yoyoCenterPos = {};
	
	/*ばね運動の振動半径*/
	FVector2D _yoyoRadius2D = {};
	
	/*ばね運動前のカッター初期位置*/
	float _offsetRad = 0.f;
	
	/*運動を始めてからの経過時間*/
	float _currentTime = 0.f;
	
	UPROPERTY() TObjectPtr<UStaticMeshComponent> _staticMeshComponent = {};
};