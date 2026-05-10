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
	virtual void Tick(float DeltaTime) override;

	/*このオブジェクトからスコアを奪う*/
	virtual FScoreRobbedParam RobbedScore_Implementation(bool isExecPlayer) override;
	virtual FSealedBaseParam* GetParam() override { return &_param; }
	
protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FSealedBaseParam _param = {};
};
