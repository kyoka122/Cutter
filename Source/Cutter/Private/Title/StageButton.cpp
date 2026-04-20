#include "StageButton.h"
#include "System/CutterWorldSubsystem.h"
#include "Application/SceneDefine.h"
#include "TableRow/StageRowData.h"

void UStageButton::Init(const FStageRowData& data)
{
	_stageName = data.levelName;
	check(IsValid(_stageText))
	_stageText->SetText(data.text);
	
	check(IsValid(_stageImage)) 
	_stageImage->SetBrushFromTexture(data.image);
}

void UStageButton::OnStageButtonClick()
{
	UE_LOG(LogTemp, Log, TEXT("ステージに移動します"));
	TObjectPtr<UCutterWorldSubsystem> titleWorldSubsystem = GetWorld()->GetSubsystem<UCutterWorldSubsystem>();
	LevelMoveContext context = SceneDefine::GetStageContexts(GetStageName());
	UE_LOG(LogTemp, Log, TEXT("context: %s"), *context.levelName.ToString());
	titleWorldSubsystem->MoveToStage(context);
}