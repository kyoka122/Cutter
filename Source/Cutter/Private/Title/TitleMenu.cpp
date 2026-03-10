#include "TitleMenu.h"
#include "StageButton.h"
#include "Components/ScrollBoxSlot.h"
#include "Layout/Margin.h"
#include "Struct/StageRowData.h"

void UTitleMenu::CreateStageList()
{
	check(IsValid(_stageDataTable));
	check(IsValid(_stageButtonClass));
	check(IsValid(_stageListScrollBox));

	static const FString contextString = FString::Printf(TEXT("StageList読み込み失敗: "));
	TArray<FStageRowData*> stageRows;

	_stageDataTable->GetAllRows<FStageRowData>(contextString, stageRows);

	for (const auto& row : stageRows)
	{
		if (!row)
		{
			UE_LOG(LogTemp, Log, TEXT("不正なRowがあります"));
		}
		TObjectPtr<UStageButton> newStageButton = CreateWidget<UStageButton>(this, _stageButtonClass);
		if (!newStageButton)
		{
			UE_LOG(LogTemp, Error, TEXT("Widget作成に失敗"));
			continue;
		}
		newStageButton->Init(*row);
		TObjectPtr<UPanelSlot> panelSlot = _stageListScrollBox->AddChild(newStageButton);
		TObjectPtr<UScrollBoxSlot> scrollBoxSlot = Cast<UScrollBoxSlot>(panelSlot);
		ensureMsgf(IsValid(scrollBoxSlot), TEXT("scrollBoxSlotの生成に失敗しました levelName: %s"), *row->levelName.ToString());
		scrollBoxSlot->SetPadding(_scrollBoxMargin);
	}
}
