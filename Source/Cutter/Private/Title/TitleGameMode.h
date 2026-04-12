#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TitleGameMode.generated.h"

class UTitleMenu;
/**
 * 
 */
UCLASS(abstract)
class ATitleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Title")
	void SetScene(UTitleMenu* titleMenu);
};
