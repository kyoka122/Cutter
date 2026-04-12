#include "SealedGenerator.h"

#include "Kismet/GameplayStatics.h"
#include "Obstacles/Sealeds/SealedBase.h"

void ASealedGenerator::RegisterGeneratePrefab(TSubclassOf<ASealedBase> prefab)
{
	_prefab = prefab;
}

void ASealedGenerator::RegisterParam(const TFunction<void(ASealedBase* sealed)>& releaseFunc)
{
	_releaseFunc = releaseFunc;
}

TObjectPtr<ASealedBase> ASealedGenerator::Generate()
{
	check(_prefab);
	TObjectPtr<ASealedBase> sealed = GetWorld()->SpawnActorDeferred<ASealedBase>(_prefab, FTransform::Identity);
	if (IsValid(sealed))
	{
		SafeDeactivate(sealed);
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
	SafeDeactivate(sealed);
	sealed->SetMeshAlphaColor(1);//MEMO: Meshの取得処理を先にしておかないとDeactivate時にエラーになるため他のDeactive処理と分ける
}

void ASealedGenerator::SafeDeactivate(TObjectPtr<ASealedBase> sealed)
{
	sealed->SetActorEnableCollision(false);
	sealed->SetActorTickEnabled(false);
	sealed->SetActorHiddenInGame(true);
}