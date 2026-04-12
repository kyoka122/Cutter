#pragma once

#include "CoreMinimal.h"
#include "CutterBase.h"
#include "GameFramework/Actor.h"
#include "InGame/Interface/Breakable.h"
#include "InGame/Interface/Throwable.h"
#include "Struct/StraightYoYoCutterCutterParam.h"
#include "StraightYoYoCutter.generated.h"

UCLASS()
class CUTTER_API AStraightYoYoCutter : public ACutterBase, public IThrowable, public IBreakable
{
	GENERATED_BODY()

public:
	AStraightYoYoCutter(){}
	virtual void Tick(float DeltaTime) override;
	virtual void Break() override;
	virtual void StartTargeting_Implementation(AActor* throwActor) override;
	virtual void Throw_Implementation() override;
	virtual FVector GetLocation_Implementation() const override { return GetActorLocation(); };

protected:
	virtual void BeginPlay() override;
	virtual FCutterBaseParam* GetParam() override { return &_param; }
	
protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FStraightYoYoCutterCutterParam _param = {};
	
private:
	void Translate(float deltaTime);
	FVector CalcPosition(float deltaTime);
	FRotator CalcRotation(float deltaTime) const;
	void OnOverlapBreakableActor(AActor* otherActor);
	void OnOverlapScoreTargetActor(AActor* otherActor) const;
	void OnOverlapDamageableActor(AActor* otherActor) const;
	void SetThrowTargetParam();
	void OnBreak();
	
private:
	UPROPERTY() TObjectPtr<UStaticMeshComponent> _staticMeshComponent = {};
	FVector2D _yoyoCenterPos = {};
	FVector2D _yoyoRadius2D = {};
	float _offsetRad = 0.f;
	float _currentTime = 0.f;
};