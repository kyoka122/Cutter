#pragma once

#include "Obstacles/Cutters/CutterBase.h"
#include "Obstacles/Sealeds/SealedBase.h"
#include "CutterSetData.generated.h"

/*
 *カッターとレリックの組み合わせ登録用構造体
 */
USTRUCT(BlueprintType)
struct FCutterSetData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FGameplayTag type = {};
	
	UPROPERTY(EditAnywhere)
	int score = 0;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASealedBase> sealedModeActor = {};
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ACutterBase> cutterModeActor = {};
};