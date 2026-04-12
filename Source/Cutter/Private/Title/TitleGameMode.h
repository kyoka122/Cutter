#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TitleGameMode.generated.h"

class UTitleMenu;

/**
 * タイトル画面のGameMode
 */
UCLASS(abstract)
class ATitleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	/*前のレベル遷移時に渡した情報を元に、指定の画面を表示する*/
	UFUNCTION(BlueprintCallable, Category = "Title")
	void SetScene(UTitleMenu* titleMenu);
};
