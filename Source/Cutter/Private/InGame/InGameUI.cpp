#include "InGame/InGameUI.h"

#include "Application/CutterFormat.h"
#include "InGame/InGameState.h"

void UInGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetScore(0, 0);
	SetTime(0);
}

 void UInGameUI::UpdateUI(const TObjectPtr<AInGameState> gameState, float deltaTime)
 {
	SetTime(gameState->GetLimitTime());
	if(_countAnimatedScore != gameState->GetScore())
	{
		SetScore(gameState->GetScore(), deltaTime);
	}
 }

void UInGameUI::SetScore(int score, float deltaTime)
{
	if (_countAnimatedScore < score)
	{
		_countAnimatedScore = FMath::Min(_countAnimatedScore + _countUpSpeed * deltaTime, score);
	}
	else if (_countAnimatedScore > score)
	{
		_countAnimatedScore = FMath::Max(_countAnimatedScore - _countUpSpeed * deltaTime, score);
	}
	
	_scoreText->SetText(CutterFormat::ConvertScoreText(_countAnimatedScore));
}

void UInGameUI::SetTime(float limitTime)
{
	FText&& formattedTime = CutterFormat::ConvertTimeText(limitTime);
	_timeText->SetText(formattedTime);
}