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
	
	/*スコア加算*/
	UFUNCTION(BlueprintCallable, Category = "InGame")
	void AddScore(int value);
	
	/*ブロワー消費*/
	UFUNCTION(BlueprintCallable, Category = "InGame")
	bool TryConsumeBlower();

	/*今盤面に存在する全てのオブジェクトのスピードを遅くする*/
	UFUNCTION(BlueprintCallable, Category = "InGame")
	void SetObstaclesSpeed(float value);
	
	/*制限時間の経過スピードを遅くする*/
	UFUNCTION(BlueprintCallable, Category = "InGame")
	void SetLimitTimeSpeed(float value);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(const float deltaTime) override;
	
protected:
	/*インゲーム中に表示するUI*/
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TSubclassOf<UInGameUI> _inGameUIClass = {};
	
	/*ゲームオーバー時に表示するUI*/
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TSubclassOf<UGameOverUI> _gameOverUIClass = {};
	
	/*ステージパラメータを管理するテーブル（生成オブジェクトのデータ等）*/
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TObjectPtr<UDataTable> _stageDataTable;
	
	/*汎用スポナー*/
	UPROPERTY(EditAnywhere, Category = "参照設定")
	TSubclassOf<AObstacleSpawner> _obstacleSpawnerClass;
	
private:
	void InstanceMember();
	void InitParam();
	void RegisterCharacterParam() const;
	void SetCursor() const;
	FStageRowData* GetStageData(FName stageName) const;
	TScriptInterface<IStageShape> GetStageShape() const;
	USceneCaptureComponent2D* GetOverViewCapture() const;

private:
	UPROPERTY() TObjectPtr<AInGameState> _inGameState = {};
	UPROPERTY() TObjectPtr<AObstacleSpawner> _obstacleSpawner = {};
	UPROPERTY() TObjectPtr<UWidgetHelper> _widgetHelper = {};
	UPROPERTY() TObjectPtr<UInGameUI> _inGameUI = {};
	UPROPERTY() TObjectPtr<UGameOverUI> _gameOverUI = {};
	bool _isActiveGameOverUI = false;
};



