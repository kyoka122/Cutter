#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TitleWorldSubsystem.generated.h"

/**
 * タイトル画面のサブシステム。
 * レベル遷移等の管理を行う。
 */
UCLASS()
class UTitleWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Title")
	void MoveToStage(FName stageName);
};
