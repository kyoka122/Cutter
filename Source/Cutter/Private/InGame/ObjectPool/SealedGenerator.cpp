#include "SealedGenerator.h"

void ASealedGenerator::RegisterGeneratePrefab(TSubclassOf<ASealedBase> prefab)
{
	_prefab = prefab;
}

TObjectPtr<ASealedBase> ASealedGenerator::Generate()
{
	check(_prefab);
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	TObjectPtr<ASealedBase> sealed = GetWorld()->SpawnActor<ASealedBase>(_prefab, spawnParams);
	Deactivate(sealed);
	return sealed;
}

void ASealedGenerator::Activate(TObjectPtr<ASealedBase> sealed, FTransform transform)
{
	sealed->SetActorTransform(transform);
	sealed->SetActorHiddenInGame(false);
}

void ASealedGenerator::Deactivate(TObjectPtr<ASealedBase> sealed)
{
	sealed->SetActorEnableCollision(false);
	sealed->SetActorTickEnabled(false);
	sealed->SetActorHiddenInGame(true);
}

void ASealedGenerator::RegisterScoreAddFunction(TFunction<void(int)>& scoreAddFunc)
{
	_scoreAddFunc = scoreAddFunc;
}
