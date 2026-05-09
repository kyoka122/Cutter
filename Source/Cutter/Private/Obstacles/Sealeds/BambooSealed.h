#pragma once

#include "CoreMinimal.h"
#include "SealedBase.h"
#include "GameFramework/Actor.h"
#include "InGame/Interface/ScoreTarget.h"
#include "Struct/SealedBaseParam.h"
#include "BambooSealed.generated.h"

/**
 * 別のカッターが触れるとカッターが自動生成するタイプのレリッククラス
 */
UCLASS()
class CUTTER_API ABambooSealed : public ASealedBase, public IScoreTarget
{
	GENERATED_BODY()

public:
	ABambooSealed();
	virtual void Tick(float DeltaTime) override;
	
	/*このオブジェクトからスコアを奪う*/
	virtual FScoreRobbedParam RobbedScore_Implementation(bool isExecPlayer) override;
	virtual FSealedBaseParam* GetParam() override { return &_param; }
	
protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FSealedBaseParam _param = {};
	
private:
	float _lifeTime = 0.f;
};