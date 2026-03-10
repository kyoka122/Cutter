#include "InGame/InGameState.h"

void AInGameState::AddScore(int value)
{
	score += value;
}

void AInGameState::ConsumeLimitTime(float value)
{
	limitTime = FMath::Max(0.0f, limitTime - value);
}

void AInGameState::SetLimitTime(float value)
{
	limitTime = FMath::Max(0.0f, value);
}

int AInGameState::GetScore()
{
	return score;
}

float AInGameState::GetLimitTime()
{
	return limitTime;
}

bool AInGameState::IsTimeOver()
{
	return limitTime <= 0;
}