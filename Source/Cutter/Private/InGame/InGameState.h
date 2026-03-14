#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "InGameState.generated.h"

/**
 * インゲームの基本情報クラス
 */
UCLASS()
class AInGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	void AddScore(int value);
	void ConsumeLimitTime(float value);
	void SetLimitTime(float value);
	void SetInitLimitTime(float value);
	int GetScore();
	float GetLimitTime();
	bool IsTimeOver();
	float GetInitLimitTime();

private:
	int score = 0;
	float initLimitTime = 0.0f;
	float limitTime = 0.f;
};
