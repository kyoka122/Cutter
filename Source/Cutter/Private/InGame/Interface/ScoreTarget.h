#pragma once

#include "CoreMinimal.h"
#include "Obstacles/Struct/ScoreRobbedParam.h"
#include "ScoreTarget.generated.h"

/**
 * 衝突によるスコア獲得が可能なインターフェース(UObject)
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UScoreTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 衝突によるスコア獲得が可能なインターフェース
 */
class CUTTER_API IScoreTarget
{
	GENERATED_BODY()
	
public:
	/*このオブジェクトからスコアを奪う*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InGame")
	FScoreRobbedParam RobbedScore(bool isExecPlayer);
	
	/*このオブジェクトからスコアを奪う*/
	virtual FScoreRobbedParam RobbedScore_Implementation(bool isExecPlayer) = 0;
};