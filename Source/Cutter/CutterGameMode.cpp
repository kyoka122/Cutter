#include "CutterGameMode.h"

//
// void ACutterGameMode::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	_inGameState = GetGameState<AInGameState>();
// 	if (!_inGameUI)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("GameStateの取得に失敗"));
// 	}
// 	
// 	_inGameUI = CreateWidget<UInGameUI>(GetWorld(), _inGameUIClass);
// 	if (!_inGameUI)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("Widget作成に失敗"));
// 	}
// 	//スコアの追加処理（プレイヤーが出来たらこっちも追加）
// 	//player.RegisterAddScoreEvent(AddScore);
// }
//
// void ACutterGameMode::Tick(const float deltaTime)
// {
// 	Super::Tick(deltaTime);
// 	
// 	_inGameState->limitTime += deltaTime;
// 	_inGameUI->UpdateUI(_inGameState, deltaTime);
// }
//
// void ACutterGameMode::InitGameState()
// {
// 	Super::InitGameState();
// 	_inGameState->limitTime = 65.999;//TODO: 後で本データを入れる
// }
//
// void ACutterGameMode::AddScore(int value)
// {
// 	_inGameState->score += value;
// }