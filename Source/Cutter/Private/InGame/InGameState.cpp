#include "InGame/InGameState.h"

void AInGameState::AddScore(int value)
{
	score = FMath::Max(0, score + value);
}

int AInGameState::GetScore() const
{
	return score;
}



void AInGameState::ConsumeLimitTime(float value)
{
	limitTime = FMath::Max(0.0f, limitTime - value);
}

void AInGameState::SetLimitTime(float value)
{
	limitTime = FMath::Max(0.0f, value);
}

void AInGameState::SetInitLimitTime(float value)
{
	initLimitTime = value;
}

float AInGameState::GetLimitTime() const
{
	return limitTime;
}

bool AInGameState::IsTimeOver() const
{
	return limitTime <= 0;
}

float AInGameState::GetInitLimitTime() const
{
	return initLimitTime;
}



void AInGameState::ConsumeBlower()
{
	blowerCount--;
}

void AInGameState::SetBlowerCount(int32 value)
{
	blowerCount = value;
}

int32 AInGameState::GetBlowerCount() const
{
	return blowerCount;
}