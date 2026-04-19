#pragma once

#include "CoreMinimal.h"
#include "SealedBase.h"
#include "GameFramework/Actor.h"
#include "InGame/Interface/ScoreTarget.h"
#include "Struct/SealedBaseParam.h"
#include "AncientScrollSealed.generated.h"

/**
 * キャラクターが触れるとカッターを入手できるレリッククラス
 */
UCLASS()
class CUTTER_API AAncientScrollSealed : public ASealedBase, public IScoreTarget
{
	GENERATED_BODY()

public:
	AAncientScrollSealed();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	/*生成時、オブジェクトプールから持ってくるため、ここで初期化する*/
	virtual void ReStart() override;
	
	/*このオブジェクトからスコアを奪う*/
	virtual FScoreRobbedParam RobbedScore_Implementation(bool isExecPlayer) override;
	
protected:
	virtual FSealedBaseParam* GetParam() override { return &_param; }
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Sealed")
	UStaticMeshComponent* GetStaticMesh();
	
protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FSealedBaseParam _param = {};

private:
	void CheckLifeTimeIsOver(float deltaTime);
	
private:
	float _lifeTime = 0.f;
};
