#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Layout/Margin.h"
#include "TitleMenu.generated.h"

/**
 * タイトル画面のメニューUIを管理する。
 * レベル遷移等の管理を行う。
 */
UCLASS()
class UTitleMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Title")
	void SetStageSelectView();
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Title")
	void CreateStageList();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UScrollBox> _stageListScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "参照設定")
	TSubclassOf<class UStageButton> _stageButtonClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "参照設定")
	TObjectPtr<UDataTable> _stageDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI設定")
	FMargin _scrollBoxMargin;
};
