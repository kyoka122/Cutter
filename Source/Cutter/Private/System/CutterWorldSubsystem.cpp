#include "CutterWorldSubsystem.h"
#include "Application/CutterFormat.h"
#include "Application/LevelMoveContext.h"
#include "Application/LevelMoveOptionDefine.h"
#include "Kismet/GameplayStatics.h"

void UCutterWorldSubsystem::MoveToStage(const LevelMoveContext& context)
{
	UE_LOG(LogTemp, Log, TEXT("遷移するシーン: %s"), *context.sceneName.ToString());
	if (context.levelName == NAME_None)
	{
		UE_LOG(LogTemp, Error, TEXT("レベルの名前がNoneの状態で渡されました"));
		return;
	}
	
	if (context.moveOption.IsEmpty())
	{
		UGameplayStatics::OpenLevel(GetWorld(), context.sceneName, true);
	}
	else
	{
		FString optionString = CutterFormat::ConvertOpenLevelOption(LevelMoveOptionDefine::SceneKey, context.moveOption);
		UGameplayStatics::OpenLevel(GetWorld(), context.levelName, true, optionString);
	}
	
}