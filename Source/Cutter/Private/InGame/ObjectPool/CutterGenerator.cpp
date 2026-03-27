#include "CutterGenerator.h"

void ACutterGenerator::RegisterGeneratePrefab(TSubclassOf<ACutterBase> prefab)
{
	_prefab = prefab;
}

TObjectPtr<ACutterBase> ACutterGenerator::Generate()
{
	check(_prefab);
	TObjectPtr<ACutterBase> cutter = GetWorld()->SpawnActor<ACutterBase>(_prefab);
	Deactivate(cutter);
	return cutter;
}

void ACutterGenerator::Activate(TObjectPtr<ACutterBase> cutter, FTransform transform)
{
	cutter->SetActorTransform(transform);
}

void ACutterGenerator::Deactivate(TObjectPtr<ACutterBase> cutter)
{
	cutter->StopTick();
	cutter->SetActorHiddenInGame(true);
	cutter->SetActorEnableCollision(false);
}
