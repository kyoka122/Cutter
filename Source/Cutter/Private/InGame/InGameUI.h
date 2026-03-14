#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "InGame/InGameState.h"
#include "InGameUI.generated.h"

/**
 * インゲームで表示するUI全てを管理するクラス
 */
UCLASS()
class UInGameUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateUI(TObjectPtr<AInGameState> gameState, float deltaTime);

protected:
	virtual void NativeConstruct() override;
	
private:
	void SetScore(int score, float deltaTime);
	void SetTime(float limitTime);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> _scoreText = {};
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> _timeText = {};
	
	UPROPERTY(EditAnywhere, Category = "UI設定")
	int _countUpSpeed = 0;

private:
	int _countAnimatedScore = 0;//MEMO: スコアのカウントアニメーション実装のため、現在表示している数値をキャッシュしておく
	float _time = 0;
};
