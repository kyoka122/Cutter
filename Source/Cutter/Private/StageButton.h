#pragma once

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Struct/StageRowData.h"
#include "StageButton.generated.h"

/**
 * タイトル画面の各ステージボタンの実態。
 */
UCLASS()
class UStageButton : public UUserWidget
{
	GENERATED_BODY()

public:
	void CreateButton(const FStageRowData& data);
	
	UFUNCTION(BlueprintCallable, Category = "Title")
	FName GetStageName() const { return _stageName; }

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> _stageImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> _stageText;

private:
	FName _stageName;
};
