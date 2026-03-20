#include "TitleGameMode.h"

#include "TitleMenu.h"
#include "Application/SceneDefine.h"
#include "Application/LevelMoveOptionDefine.h"
#include "Kismet/GameplayStatics.h"

void ATitleGameMode::SetScene(UTitleMenu* titleMenu)
{
	FName sceneName = FName(UGameplayStatics::ParseOption(OptionsString, LevelMoveOptionDefine::SceneKey));
	UE_LOG(LogTemp, Log, TEXT("遷移先シーン: %s"), *sceneName.ToString());
	if (sceneName == SceneDefine::StageSelect)
	{
		titleMenu->SetStageSelectView();
	}
}
