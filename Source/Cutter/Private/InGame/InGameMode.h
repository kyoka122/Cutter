#pragma once

#include "ObstacleSpawner.h"
#include "InGame/GameOverUI.h"
#include "InGame/InGameState.h"
#include "InGame/InGameUI.h"
#include "GameFramework/GameModeBase.h"
#include "Struct/StageRowData.h"
#include "Utility/WidgetHelper.h"
#include "InGameMode.generated.h"

/**
 *  インゲームを管理するGameModeクラス
 */
UCLASS(abstract)
class AInGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AInGameMode();
	
	UFUNCTION(BlueprintCallable, Category = "InGame")
	void AddScore(int value);
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(const float deltaTime) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TSubclassOf<UInGameUI> _inGameUIClass = {};
	
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TSubclassOf<UGameOverUI> _gameOverUIClass = {};
	
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TObjectPtr<UDataTable> _stageDataTable;
	
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TSubclassOf<AObstacleSpawner> _obstacleSpawnerClass;
	
private:
	void InstanceMember();
	void InitParam();
	void SetCursor();
	FStageRowData* GetStageData(FName stageName);
	
private:
	TObjectPtr<AInGameState> _inGameState = {};
	//TObjectPtr<AObstacleManager> _obstacleManager = {};
	TObjectPtr<AObstacleSpawner> _obstacleSpawner = {};
	TObjectPtr<UWidgetHelper> _widgetHelper = {};
	TObjectPtr<UInGameUI> _inGameUI = {};
	TObjectPtr<UGameOverUI> _gameOverUI = {};
	bool _isActiveGameOverUI = false;
};



