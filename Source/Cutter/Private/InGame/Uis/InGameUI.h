#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGame/Interface/OverViewMiniMap.h"
#include "InGameUI.generated.h"

class AInGameState;
class UImage;
class UTextBlock;

/**
 * インゲームで表示するUI全てを管理するクラス
 */
UCLASS()
class UInGameUI : public UUserWidget, public IOverViewMiniMap
{
	GENERATED_BODY()
	
public:
	void UpdateUI(TObjectPtr<AInGameState> gameState, float deltaTime);
	virtual void SetVisibilityMiniMap(bool value) override;
	
protected:
	virtual void NativeConstruct() override;
	
private:
	void SetScore(int score, float deltaTime);
	void SetTime(float limitTime) const;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> _scoreText = {};
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> _timeText = {};
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> _miniMap = {};
	
	UPROPERTY(EditAnywhere, Category = "UI設定")
	float _countUpSpeed = 1;

private:
	float _countAnimatedScore = 0;//MEMO: スコアのカウントアニメーション実装のため、現在表示している数値をキャッシュしておく
	float _time = 0;
};
