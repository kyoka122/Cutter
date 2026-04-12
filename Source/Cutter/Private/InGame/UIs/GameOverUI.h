#pragma once

#include "Blueprint/UserWidget.h"
#include "GameOverUI.generated.h"

class AInGameState;
class UTextBlock;

/**
 * ゲームオーバー時に表示するUI（スコア表示等）
 */
UCLASS()
class UGameOverUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/*表示コンテンツの更新*/
	void Update(const AInGameState* gameState) const;
	
protected:
	/*ステージ選択画面へ遷移*/
	UFUNCTION(BlueprintCallable, Category = "InGame")
	void MoveToStageSelect();
	
	/*リトライのため同じレベルに遷移*/
	UFUNCTION(BlueprintCallable, Category = "InGame")
	void Retry();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> _scoreText;
};
