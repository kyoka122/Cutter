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
	virtual int RobbedScore_Implementation(bool isExecPlayer) override;
	

protected:
	//void RegisterEvent(UStaticMeshComponent* staticMeshComponent,OverlapFunc func);
	//void RegisterStaticMeshEvent(UStaticMeshComponent* staticMeshComponent, OverlapFunc func);

private:
	
	// UFUNCTION()
	// virtual void OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	// 	UPrimitiveComponent* OtherComp, int32 Ot
};
