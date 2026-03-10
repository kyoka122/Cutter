#pragma once

#include "InGame/GameOverUI.h"
#include "InGame/InGameState.h"
#include "InGame/InGameUI.h"
#include "GameFramework/GameModeBase.h"
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
protected:
	virtual void BeginPlay() override;
	virtual void Tick(const float deltaTime) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "参照設定")
	TSubclassOf<UInGameUI> _inGameUIClass = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "参照設定")
	TSubclassOf<UGameOverUI> _gameOverUIClass = {};
	
private:
	void InstanceMember();
	void InitParam();
	void AddScore(int value);
	
private:
	TObjectPtr<AInGameState> _inGameState = {};
	TObjectPtr<UWidgetHelper> _widgetHelper = {};
	TObjectPtr<UInGameUI> _inGameUI = {};
	TObjectPtr<UGameOverUI> _gameOverUI = {};
	bool _isActiveGameOverUI = false;
};



