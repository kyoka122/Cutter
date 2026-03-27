#include "SealedGenerator.h"

void ASealedGenerator::RegisterGeneratePrefab(TSubclassOf<ASealedBase> prefab)
{
	_prefab = prefab;
}

TObjectPtr<ASealedBase> ASealedGenerator::Generate()
{
	TObjectPtr<ASealedBase> cutter = GetWorld()->SpawnActor<ASealedBase>(_prefab);
	Deactivate(cutter);
	return cutter;
}

void ASealedGenerator::Activate(TObjectPtr<ASealedBase> cutter, FTransform transform)
{
	cutter->SetActorTransform(transform);
	cutter->SetActorHiddenInGame(false);
}

void ASealedGenerator::Deactivate(TObjectPtr<ASealedBase> cutter)
{
	cutter->StopTick();
	cutter->SetActorHiddenInGame(true);
}

void ASealedGenerator::RegisterScoreAddFunction(TFunction<void(int)>& scoreAddFunc)
{
	_scoreAddFunc = scoreAddFunc;
}
