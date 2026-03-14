#include "InGame/GameOverUI.h"
#include "InGameState.h"
#include "Application/CutterFormat.h"
#include "Application/SceneDefine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "System/CutterWorldSubsystem.h"

void UGameOverUI::Update(const TObjectPtr<AInGameState> gameState)
{
	_scoreText->SetText(CutterFormat::ConvertScoreText(gameState->GetScore()));
}

void UGameOverUI::MoveToStageSelect()
{
	UE_LOG(LogTemp, Log, TEXT("ステージ選択に移動します"));
	TObjectPtr<UCutterWorldSubsystem> titleWorldSubsystem = GetWorld()->GetSubsystem<UCutterWorldSubsystem>();
	LevelMoveContext context = SceneDefine::GetStageContexts(SceneDefine::StageSelect);
	titleWorldSubsystem->MoveToStage(context);
}

void UGameOverUI::Retry()
{
	UE_LOG(LogTemp, Log, TEXT("リトライします"));
	TObjectPtr<UCutterWorldSubsystem> titleWorldSubsystem = GetWorld()->GetSubsystem<UCutterWorldSubsystem>();
	FName currentLevelName = FName(UKismetSystemLibrary::GetDisplayName(GetWorld()));
	LevelMoveContext context = SceneDefine::GetStageContexts(currentLevelName);
	titleWorldSubsystem->MoveToStage(context);
}