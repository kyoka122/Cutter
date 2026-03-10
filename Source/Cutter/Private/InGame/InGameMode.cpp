#pragma once

#include "InGame/InGameMode.h"
#include "InGame/GameOverUI.h"
#include "InGame/InGameState.h"
#include "InGame/InGameUI.h"
#include "Blueprint/UserWidget.h"
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
	
	_inGameUI = _widgetHelper->View<UInGameUI>(_inGameUIClass);
}

void AInGameMode::InitParam()
{
	_inGameState->SetLimitTime(8.999); //TODO: 後で本データを入れる
	
	//スコアの追加処理（プレイヤーが出来たらこっちも追加）
	//player.RegisterAddScoreEvent(AddScore);
}

void AInGameMode::Tick(const float deltaTime)
{
	Super::Tick(deltaTime);
	
	_inGameState->ConsumeLimitTime(deltaTime);
	_inGameUI->UpdateUI(_inGameState, deltaTime);

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

