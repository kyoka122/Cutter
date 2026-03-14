#pragma once

#include "InGame/InGameMode.h"
//#include "ObstacleManager.h"
#include "InGame/GameOverUI.h"
#include "InGame/InGameState.h"
#include "InGame/InGameUI.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Struct/StageRowData.h"
#include "Utility/WidgetHelper.h"

AInGameMode::AInGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInGameMode::BeginPlay()
{
	Super::BeginPlay();
	InstanceMember();
	InitParam();
}

void AInGameMode::InstanceMember()
{
	_widgetHelper = GetWorld()->GetSubsystem<UWidgetHelper>();
	check(IsValid(_widgetHelper));
	_inGameState = GetGameState<AInGameState>();
	check(IsValid(_inGameState));
	//_obstacleManager = GetWorld()->SpawnActor<AObstacleManager>(AObstacleManager::StaticClass());
	_obstacleSpawner = GetWorld()->SpawnActor<AObstacleSpawner>(_obstacleSpawnerClass);
	check(IsValid(_obstacleSpawner));
	
	_inGameUI = _widgetHelper->View<UInGameUI>(_inGameUIClass);
}

void AInGameMode::InitParam()
{
	FName currentLevel = FName(UKismetSystemLibrary::GetDisplayName(GetWorld()));
	FStageRowData* row = GetStageData(currentLevel);
	check(row);
	
	_obstacleSpawner->Init(row->obstacleSpawnData);
	_inGameState->SetInitLimitTime(row->limitTime);
	_inGameState->SetLimitTime(row->limitTime);
	//スコアの追加処理（プレイヤーが出来たらこっちも追加）
	//player.RegisterAddScoreEvent(AddScore);
}

FStageRowData* AInGameMode::GetStageData(FName stageName)//TODO: Factory作る
{
	FString contextString = FString::Printf(TEXT("StageList読み込み失敗: "));
	TArray<FStageRowData*> stageRows;
	_stageDataTable->GetAllRows<FStageRowData>(contextString, stageRows);
	for (const auto& row : stageRows)
	{
		if (!row)
		{
			UE_LOG(LogTemp, Log, TEXT("不正なRowがあります"));
			continue;
		}
		if (row->levelName == stageName)
		{
			return row;
		}
	}
	return nullptr;
}

void AInGameMode::Tick(const float deltaTime)
{
	Super::Tick(deltaTime);
	
	_inGameState->ConsumeLimitTime(deltaTime);
	_inGameUI->UpdateUI(_inGameState, deltaTime);
	_obstacleSpawner->Update(_inGameState);
	if (!_isActiveGameOverUI && _inGameState->IsTimeOver())
	{
		_isActiveGameOverUI = true;
		_gameOverUI = _widgetHelper->View<UGameOverUI>(_gameOverUIClass);
		_gameOverUI->Update(_inGameState);
		_widgetHelper->SetInteractiveMode(_gameOverUI);
	}
}

void AInGameMode::AddScore(int value)
{
	if (_inGameState->GetLimitTime() > 0)
	{
		_inGameState->AddScore(value);
	}
}

