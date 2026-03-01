#include "TitleMenu.h"

#include "StageButton.h"
#include "Components/ScrollBoxSlot.h"
#include "Layout/Margin.h"
#include "Struct/StageRowData.h"

void UTitleMenu::CreateStageList()
{
	if (!_stageDataTable || !_stageButtonClass)
	{
		return;
	}

	static const FString contextString = FString::Printf(TEXT("StageList読み込み失敗: "));
	TArray<FStageRowData*> stageRows;

	_stageDataTable->GetAllRows<FStageRowData>(contextString, stageRows);

	for (const FStageRowData* row : stageRows)
	{
		if (row)
		{
			UStageButton* newStageButton = CreateWidget<UStageButton>(this, _stageButtonClass);
			newStageButton->CreateButton(*row);
			UPanelSlot* panelSlot = _stageListScrollBox->AddChild(newStageButton);
			UScrollBoxSlot* scrollBoxSlot = Cast<UScrollBoxSlot>(panelSlot);
			scrollBoxSlot->SetPadding(_scrollBoxMargin);
			
		}else
		{
			UE_LOG(LogTemp, Error, TEXT("Rowが見つかりません"));
		}
	}
}
