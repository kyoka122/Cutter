#pragma once

#include "CoreMinimal.h"
#include "LevelMoveContext.h"

/**
 * 
 */
class SceneDefine
{
public:
	static LevelMoveContext GetStageContexts(const FName& scene)
	{
		for (LevelMoveContext stageContext : GetDefaultStageContexts())
		{
			if (stageContext.sceneName == scene)
			{
				return stageContext;
			}
		}
		
		//stageContextsに当てはまらなかったものはInGameのStage系
		return LevelMoveContext(scene,scene);
	}
	
public:
	static FName Title;
	static FName StageSelect;
	
private:
	static TArray<LevelMoveContext> GetDefaultStageContexts()
	{
		return TArray
		{
			LevelMoveContext(Title,FName("Title")),
			LevelMoveContext(StageSelect,FName("Title"), FString("StageSelect")),
		};
	}
};
