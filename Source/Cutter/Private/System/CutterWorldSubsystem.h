#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CutterWorldSubsystem.generated.h"

struct LevelMoveContext;

/**
 * タイトル画面のサブシステム。
 * レベル遷移等の管理を行う。
 */
UCLASS()
class UCutterWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void MoveToStage(const LevelMoveContext& context) const;
};
