#pragma once

#include "Blueprint/UserWidget.h"
#include "GameOverUI.generated.h"

class AInGameState;
class UTextBlock;
/**
 * 
 */
UCLASS()
class UGameOverUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Update(const TObjectPtr<AInGameState> gameState) const;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "InGame")
	void MoveToStageSelect();
	
	UFUNCTION(BlueprintCallable, Category = "InGame")
	void Retry();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> _scoreText;
	
};
