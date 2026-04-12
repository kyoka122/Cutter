#pragma once

#include "CoreMinimal.h"
#include "LevelMoveContext.h"

/**
 * 遷移パターンから遷移レベルやContextを導出するConvertクラス
 */
class SceneDefine
{
public:
	/*遷移パラメータ*/
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

FName SceneDefine::Title = "Title";
FName SceneDefine::StageSelect = "StageSelect";