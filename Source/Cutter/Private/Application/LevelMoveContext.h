#pragma once

#include "CoreMinimal.h"

/**
 * レベル遷移時のパラメータ定義
 */
struct LevelMoveContext
{
public:
	/* レベル遷移時のパラメータ（オプション付き）*/
	LevelMoveContext(FName scene, FName level, FString option)
	{
		sceneName = scene;
		levelName = level;
		moveOption = option;
	}
	
	/* レベル遷移時のパラメータ */
	LevelMoveContext(FName scene, FName level)
	{
		sceneName = scene;
		levelName = level;
	}
	
public:
	FName sceneName = {};
	FName levelName = {};
	FString moveOption = {};
};
