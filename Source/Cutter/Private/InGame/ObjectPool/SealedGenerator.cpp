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

ASealedBase* ASealedGenerator::Generate()
{
	check(_prefab);
	ASealedBase* sealed = GetWorld()->SpawnActorDeferred<ASealedBase>(_prefab, FTransform::Identity);
	if (IsValid(sealed))
	{
		SafeDeactivate(sealed);
		UGameplayStatics::FinishSpawningActor(sealed, FTransform::Identity);
		sealed->SetActorTickEnabled(false);//MEMO: Spawn後にTickが始まってしまうので、再度OFF
		sealed->RegisterReleaseFunc(_releaseFunc);
		return sealed;
	}
	UE_LOG(LogTemp, Error, TEXT("オブジェクトを生成できませんでした。 Generator: SealedGenerator"));
	return nullptr;
}

void ASealedGenerator::Activate(ASealedBase* sealed, const FTransform& transform)
{
	sealed->SetActorTransform(transform);
	Activate(sealed);
}

void ASealedGenerator::Activate(ASealedBase* sealed)
{
	sealed->SetActorHiddenInGame(false);
	sealed->SetActorEnableCollision(true);
	sealed->SetActorTickEnabled(true);
}

void ASealedGenerator::Deactivate(ASealedBase* sealed)
{
	SafeDeactivate(sealed);
	
	//MEMO: Meshの取得処理を先にしておかないとDeactivate時にエラーになるためSafeDeactivate処理と分ける
	sealed->SetMeshAlphaColor(1);
	sealed->ClearAllAnimation();
}

void ASealedGenerator::SafeDeactivate(ASealedBase* sealed)
{
	sealed->SetActorEnableCollision(false);
	sealed->SetActorTickEnabled(false);
	sealed->SetActorHiddenInGame(true);
}