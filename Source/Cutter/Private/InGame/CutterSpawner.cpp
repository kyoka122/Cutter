#include "CutterSpawner.h"

#include "DataAsset/CutterListDataAsset.h"
#include "Obstacles/CutterBase.h"

ACutterSpawner::ACutterSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACutterSpawner::RegisterCutterData(TFunction<void(int)> scoreAddFunc)
{
	_scoreAddFunc = scoreAddFunc;
}

