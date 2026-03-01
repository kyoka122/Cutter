#include "StageButton.h"

#include "Struct/StageRowData.h"

void UStageButton::CreateButton(const FStageRowData& data)
{
	_stageName = data.levelName;
	if (_stageText) _stageText->SetText(data.text);
	if (_stageImage) _stageImage->SetBrushFromTexture(data.image);
}
