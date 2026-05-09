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
	//スコア処理/////////////////////////////////
	void AddScore(int value);
	int GetScore() const;
	
	//制限時間処理///////////////////////////////
	void ConsumeLimitTime(float value);
	void SetLimitTime(float value);
	float GetLimitTime() const;
	/*制限時間を過ぎているかどうか*/
	bool IsTimeOver() const;
	
	/*制限時間初回設定*/
	void SetInitLimitTime(float value);
	/*最初に設定した制限時間*/
	float GetInitLimitTime() const;
	
	//ブロワー処理//////////////////////////////
	void ConsumeBlower();
	void SetBlowerCount(int32 value);
	int32 GetBlowerCount() const;
	
private:
	int score = 0;
	float initLimitTime = 0.0f;
	float limitTime = 0.f;
	int32 blowerCount = 0;
};
