#pragma once

#include "GameFramework/GameModeBase.h"
#include "InGameMode.generated.h"

struct FStageRowData;
class IStageShape;
class UWidgetHelper;
class AInGameState;
class AObstacleSpawner;
class UGameOverUI;
class UInGameUI;

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
	void RegisterMiniMapToCharacter() const;
	void SetCursor() const;
	FStageRowData* GetStageData(FName stageName) const;
	TScriptInterface<IStageShape> GetStageShape() const;

private:
	UPROPERTY() TObjectPtr<AInGameState> _inGameState = {};
	UPROPERTY() TObjectPtr<AObstacleSpawner> _obstacleSpawner = {};
	UPROPERTY() TObjectPtr<UWidgetHelper> _widgetHelper = {};
	UPROPERTY() TObjectPtr<UInGameUI> _inGameUI = {};
	UPROPERTY() TObjectPtr<UGameOverUI> _gameOverUI = {};
	
	bool _isActiveGameOverUI = false;
};



