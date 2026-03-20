#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct LevelMoveContext
{
public:
	LevelMoveContext(FName scene, FName level, FString option)
	{
		sceneName = scene;
		levelName = level;
		moveOption = option;
	}
	
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
