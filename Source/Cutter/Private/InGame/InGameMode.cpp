#pragma once

#include "InGame/InGameMode.h"
#include "InGame/GameOverUI.h"
#include "InGame/InGameState.h"
#include "InGame/InGameUI.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TableRow/StageEnvironmentParam.h"
#include "TableRow/StageRowData.h"
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
	SetCursor();
}

void AInGameMode::InstanceMember()
{
	_widgetHelper = GetWorld()->GetSubsystem<UWidgetHelper>();
	check(IsValid(_widgetHelper));
	_inGameState = GetGameState<AInGameState>();
	check(IsValid(_inGameState));
	_obstacleSpawner = GetWorld()->SpawnActor<AObstacleSpawner>(_obstacleSpawnerClass);
	check(IsValid(_obstacleSpawner));
	
	_inGameUI = _widgetHelper->View<UInGameUI>(_inGameUIClass);
}

void AInGameMode::InitParam()
{
	FName currentLevel = FName(UKismetSystemLibrary::GetDisplayName(GetWorld()));
	FStageRowData* stageRowData = GetStageData(currentLevel);
	check(stageRowData);
	
	FStageEnvironmentParam* stageEnvironmentParamRowData= GetStageEnvironmentParam(currentLevel);
	check(stageEnvironmentParamRowData);
	
	_obstacleSpawner->Init(stageRowData->obstacleSpawnData, stageEnvironmentParamRowData, [this](int score)
	{
		AddScore(score);
	});
	_inGameState->SetInitLimitTime(stageRowData->limitTime);
	_inGameState->SetLimitTime(stageRowData->limitTime);
}

void AInGameMode::SetCursor()
{
	TObjectPtr<APlayerController> playerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(playerController))
	{
		FInputModeGameOnly InputMode = {};
		playerController->SetInputMode(InputMode);
		playerController->bShowMouseCursor = false;
	}
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
	UE_LOG(LogTemp, Log, TEXT("該当するデータがありません。name:%s"), *stageName.ToString());
	return nullptr;
}

FStageEnvironmentParam* AInGameMode::GetStageEnvironmentParam(FName stageName)//TODO: Factory作る
{
	FString contextString = FString::Printf(TEXT("StageList読み込み失敗: "));
	TArray<FStageEnvironmentParam*> stageRows;
	_stageEnvironmentParamTable->GetAllRows<FStageEnvironmentParam>(contextString, stageRows);
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
	UE_LOG(LogTemp, Log, TEXT("該当するデータがありません。name:%s"), *stageName.ToString());
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

