#pragma once

#include "CoreMinimal.h"
#include "SealedBase.h"
#include "GameFramework/Actor.h"
#include "InGame/Interface/ScoreTarget.h"
#include "AncientScrollSealed.generated.h"

UCLASS()
class CUTTER_API AAncientScrollSealed : public ASealedBase, public IScoreTarget
{
	GENERATED_BODY()

public:
	AAncientScrollSealed();
	virtual void ReStart() override;
	virtual FScoreRobbedParam RobbedScore_Implementation(bool isExecPlayer) override;
};
