#include "SealedGenerator.h"
#include "Kismet/GameplayStatics.h"
#include "Obstacles/SealedBase.h"

void ASealedGenerator::RegisterGeneratePrefab(TSubclassOf<ASealedBase> prefab)
{
	_prefab = prefab;
}

void ASealedGenerator::RegisterParam(TFunction<void(ASealedBase* sealed)> releaseFunc)
{
	_releaseFunc = releaseFunc;
}

TObjectPtr<ASealedBase> ASealedGenerator::Generate()
{
	check(_prefab);
	TObjectPtr<ASealedBase> sealed = GetWorld()->SpawnActorDeferred<ASealedBase>(_prefab, FTransform::Identity);
	if (IsValid(sealed))
	{
		Deactivate(sealed);
		UGameplayStatics::FinishSpawningActor(sealed, FTransform::Identity);
		sealed->SetActorTickEnabled(false);//MEMO: Spawn後にTickが始まってしまうので、再度OFF
		sealed->RegisterReleaseFunc(_releaseFunc);
		return sealed;
	}
	UE_LOG(LogTemp, Log, TEXT("オブジェクトを生成できませんでした。 Generator: SealedGenerator"));
	return nullptr;
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
	sealed->SetMeshAlphaColor(1);
}