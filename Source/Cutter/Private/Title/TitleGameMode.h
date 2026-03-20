#pragma once

#include "CoreMinimal.h"
#include "TitleMenu.h"
#include "GameFramework/GameModeBase.h"
#include "TitleGameMode.generated.h"

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
