#pragma once

#include "InGameState.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameOverUI.generated.h"

/**
 * 
 */
UCLASS()
class UGameOverUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Update(const TObjectPtr<AInGameState> gameState);
	
protected:
	UFUNCTION(BlueprintCallable, Category = "InGame")
	void MoveToStageSelect();
	
	UFUNCTION(BlueprintCallable, Category = "InGame")
	void Retry();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> _scoreText;
	
};
