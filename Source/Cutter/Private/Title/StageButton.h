#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Struct/StageRowData.h"
#include "StageButton.generated.h"

/**
 * タイトル画面の各ステージ単位のUI。
 */
UCLASS()
class UStageButton : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(const FStageRowData& data);
	FName GetStageName() const { return _stageName; }
	
	UFUNCTION(BlueprintCallable, Category = "Title")
	void MoveToStage();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> _stageImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> _stageText;

private:
	FName _stageName;
};
