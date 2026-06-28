#include "InGame/InGameMode.h"

#include "CutterCharacter.h"
#include "ObstacleSpawner.h"
#include "Components/SceneCaptureComponent2D.h"
#include "InGame/Uis/GameOverUI.h"
#include "InGame/InGameState.h"
#include "InGame/Interface/OverViewMiniMap.h"
#include "InGame/Interface/CutterLooksView.h"
#include "InGame/Uis/InGameUI.h"
#include "Engine/LevelScriptActor.h"
#include "Interface/StageProperty.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Stage/StageShape.h"
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
	CachePlayer();
	InitParam();
	RegisterCharacterParam();
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

void AInGameMode::CachePlayer()
{
	ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (ACutterCharacter* cutterCharacter = Cast<ACutterCharacter>(character))
	{
		_player = cutterCharacter;
	}
	else UE_LOG(LogTemp, Error, TEXT("PlayerCharacterをCutterCharacterにCastできませんでした。"));
}

void AInGameMode::InitParam()
{
	FName currentLevel = FName(UKismetSystemLibrary::GetDisplayName(GetWorld()));
	FStageRowData* stageRowData = GetStageData(currentLevel);
	check(stageRowData);
	
	TScriptInterface<IStageShape> stageEnvironmentParamRowData = GetStageShape();
	check(stageEnvironmentParamRowData);
	
	_obstacleSpawner->Init(
		stageRowData->obstacleSpawnData,
		stageEnvironmentParamRowData,
		[this](int score){AddScore(score);},
		_player);
	_inGameState->SetInitLimitTime(stageRowData->limitTime);
	_inGameState->SetLimitTime(stageRowData->limitTime);
	_inGameState->SetBlowerCount(stageRowData->blowerCount);
	SetOverViewCapture();
}

void AInGameMode::RegisterCharacterParam() const
{
	if (IsValid(_player))
	{
		if (IsValid(_inGameUI) && _inGameUI->Implements<UOverViewMiniMap>())
		{
			if (_inGameUI->Implements<UCutterLooksView>())
			{
				_player->RegisterUIs(_inGameUI, _overViewCapture, _inGameUI);
			}
			else UE_LOG(LogTemp, Error, TEXT("_inGameUIをUCutterLooksViewにCastできませんでした。"));
		}
		else UE_LOG(LogTemp, Error, TEXT("_inGameUIをUOverViewMiniMapにCastできませんでした。"));
		
		_player->RegisterStageShape(GetStageShape());
	}
	else UE_LOG(LogTemp, Error, TEXT("_playerがnullでした。"));
}

void AInGameMode::SetCursor() const
{
	TObjectPtr<APlayerController> playerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(playerController))
	{
		FInputModeGameOnly InputMode = {};
		playerController->SetInputMode(InputMode);
		playerController->bShowMouseCursor = false;
	}
}

FStageRowData* AInGameMode::GetStageData(FName stageName) const
{
	FString contextString = FString::Printf(TEXT("StageList読み込み失敗: "));
	TArray<FStageRowData*> stageRows;
	_stageDataTable->GetAllRows<FStageRowData>(contextString, stageRows);
	for (const auto& row : stageRows)
	{
		if (!row)
		{
			UE_LOG(LogTemp, Error, TEXT("不正なRowがあります"));
			continue;
		}
		if (row->levelName == stageName)
		{
			return row;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("該当するデータがありません。name:%s"), *stageName.ToString());
	return nullptr;
}

TScriptInterface<IStageShape> AInGameMode::GetStageShape() const
{
	ALevelScriptActor* levelScriptActor = GetWorld()->GetLevelScriptActor();
	if (levelScriptActor->Implements<UStageProperty>())
	{
		TScriptInterface<IStageShape> stageShape = IStageProperty::Execute_GetStageShape(levelScriptActor);
		return stageShape;
	}
	UE_LOG(LogTemp, Error, TEXT("IStagePropertyがレベルブループリントに実装されていません"));
	return nullptr;
}

void AInGameMode::SetOverViewCapture()
{
	ALevelScriptActor* levelScriptActor = GetWorld()->GetLevelScriptActor();
	if (levelScriptActor->Implements<UStageProperty>())
	{
		_overViewCapture = IStageProperty::Execute_GetOverViewCapture(levelScriptActor);
	}
	else UE_LOG(LogTemp, Error, TEXT("GetOverViewCamera()がレベルブループリントに実装されていません"));
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

bool AInGameMode::TryConsumeBlower()
{
	if (_inGameState->GetBlowerCount() > 0)
	{
		_inGameState->ConsumeBlower();
		return true;
	}
	return false;
}

void AInGameMode::SetObstaclesSpeed(float value)
{
	for (auto& obstacle : _obstacleSpawner->GetCurrentUsingObstacles())
	{
		obstacle->CustomTimeDilation = value;
	}
}

void AInGameMode::UpdateMinimap()
{
	TArray<FVector2D> points;
	for (auto& bamboo : _obstacleSpawner->GetCurrentUsingBamboos())
	{
		points.Add(FVector2D(bamboo->GetActorLocation()));
	}
	_inGameUI->UpdateBambooIcon(points, _overViewCapture->GetComponentLocation(), _overViewCapture->OrthoWidth);
}

void AInGameMode::SetLimitTimeSpeed(float value)
{
	CustomTimeDilation = value;
}
