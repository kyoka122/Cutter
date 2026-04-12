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
	/*ステージ選択画面を表示する*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Title")
	void SetStageSelectView();
	
protected:
	/*データから読みだした通りにステージボタンを配置する*/
	UFUNCTION(BlueprintCallable, Category = "Title")
	void CreateStageList();

protected:
	/*ステージ選択のスクロール表示*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UScrollBox> _stageListScrollBox;

	/*複製に使用するボタンウィジェットクラス*/
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TSubclassOf<class UStageButton> _stageButtonClass;

	/*ステージの情報が登録されたテーブル*/
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TObjectPtr<UDataTable> _stageDataTable;

	/*スクロール内のボタン同士のマージン*/
	UPROPERTY(EditAnywhere, Category = "UI設定")
	FMargin _scrollBoxMargin;
};
