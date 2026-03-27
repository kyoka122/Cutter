#include "SealedBase.h"

ASealedBase::ASealedBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASealedBase::StartTick()
{
	SetActorTickEnabled(true);
}

void ASealedBase::StopTick()
{
	SetActorTickEnabled(false);
}

void ASealedBase::RegisterTransformCutterData(int score, FGameplayTag type, TransformCutterFunc transformCutterFunc)
{
	_score = score;
	_type= type;
	_transformCutterFunc = transformCutterFunc;
}

