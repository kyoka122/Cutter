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
	int GetScore();
	float GetLimitTime();
	bool IsTimeOver();

private:
	int score = 0;
	float limitTime = 0.f;
};
