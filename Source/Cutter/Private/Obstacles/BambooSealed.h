#pragma once

#include "CoreMinimal.h"
#include "SealedBase.h"
#include "GameFramework/Actor.h"
#include "InGame/Interface/ScoreTarget.h"
#include "Struct/SealedBaseParam.h"
#include "BambooSealed.generated.h"

UCLASS()
class CUTTER_API ABambooSealed : public ASealedBase, public IScoreTarget
{
	GENERATED_BODY()

public:
	ABambooSealed();
	virtual void Tick(float DeltaTime) override;
	virtual void ReStart() override;
	void CheckLifeTimeIsOver(float deltaTime);
	virtual FScoreRobbedParam RobbedScore_Implementation(bool isExecPlayer) override;
	
protected:
	UPROPERTY(EditAnywhere, meta=(ShowOnlyInnerProperties))
	FSealedBaseParam _param = {};
	
private:
	float _lifeTime = 0.f;
};
