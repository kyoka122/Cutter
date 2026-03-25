#pragma once

#include "CoreMinimal.h"
#include "SealedBase.h"
#include "GameFramework/Actor.h"
#include "InGame/Interface/ScoreTarget.h"
#include "BambooSealed.generated.h"

UCLASS()
class CUTTER_API ABambooSealed : public ASealedBase, public IScoreTarget
{
	GENERATED_BODY()

public:
	ABambooSealed();
	virtual int RobbedScore_Implementation(bool isExecPlayer) override;
};
