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
	/*スコア加算ロジック*/
	void AddScore(int value);
	
	/*制限時間消費ロジック*/
	void ConsumeLimitTime(float value);
	
	/*制限時間更新ロジック*/
	void SetLimitTime(float value);
	
	/*制限時間初回設定ロジック*/
	void SetInitLimitTime(float value);
	
	/*スコア取得ロジック*/
	int GetScore() const;
	
	/*残り制限時間取得ロジック*/
	float GetLimitTime() const;
	
	/*制限時間を過ぎているかどうか*/
	bool IsTimeOver() const;
	
	/*最初に設定した制限時間*/
	float GetInitLimitTime() const;

private:
	int score = 0;
	float initLimitTime = 0.0f;
	float limitTime = 0.f;
};
