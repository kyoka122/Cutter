#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetHelper.generated.h"

/**
 * Widgetのスポナー
 */
UCLASS()
class UWidgetHelper : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	/**
	 * ウィジェットを表示する
	 */
	template <typename WidgetT = UUserWidget> //MEMO: テンプレートなので実装もheaderに定義する
	WidgetT* View(TSubclassOf<UUserWidget> userWidgetClass = WidgetT::StaticClass())
	{
		static_assert(TIsDerivedFrom<WidgetT, UUserWidget>::IsDerived, "テンプレートにはUUserWidgetのサブクラスを指定してください");
	
		WidgetT* userWidget = CreateWidget<WidgetT>(GetWorld()->GetFirstPlayerController(), userWidgetClass);
		check(IsValid(userWidget));
		userWidget->AddToViewport();
		return userWidget;
	}
	
	/**
	 * 引数のUIを操作可能な状態にする
	 */
	UFUNCTION(BlueprintCallable, Category = "WidgetHelper")
	void SetInteractiveMode(UWidget* inWidgetToFocus)
	{
		TObjectPtr<APlayerController> playerController = GetWorld()->GetFirstPlayerController();
		if (IsValid(playerController))
		{
			FInputModeUIOnly InputMode = {};
			InputMode.SetWidgetToFocus(inWidgetToFocus->GetCachedWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		
			playerController->SetInputMode(InputMode);
			playerController->bShowMouseCursor = true;
		}
	}
};
