#include "CannonGenerator.h"

#include "Kismet/GameplayStatics.h"
#include "Obstacles/Cannon/Cannon.h"

void ACannonGenerator::RegisterGeneratePrefab(TSubclassOf<ACannon> prefab)
{
	_prefab = prefab;
}

void ACannonGenerator::RegisterParam(const TFunction<void(int)>& scoreAddFunc, const TFunction<void(ACannon* cannon)>& releaseFunc)
{
	_scoreAddFunc = scoreAddFunc;
	_releaseFunc = releaseFunc;
}

ACannon* ACannonGenerator::Generate()
{
	check(_prefab);
	ACannon* cannon = GetWorld()->SpawnActorDeferred<ACannon>(_prefab, FTransform::Identity);
	if (IsValid(cannon))
	{
		Deactivate(cannon);
		UGameplayStatics::FinishSpawningActor(cannon, FTransform::Identity);
		cannon->SetActorTickEnabled(false);//MEMO: Spawn後にTickが始まってしまうので、再度OFF
		cannon->RegisterReleaseFunc(_releaseFunc);
		return cannon;
	}
	UE_LOG(LogTemp, Error, TEXT("オブジェクトを生成できませんでした。 Generator: cannonGenerator"));
	return nullptr;
}

void ACannonGenerator::Activate(ACannon* cannon, const FTransform& transform)
{
	cannon->SetActorTransform(transform);
}

void ACannonGenerator::Deactivate(ACannon* cannon)
{
	cannon->SetActorTickEnabled(false);
	cannon->SetActorHiddenInGame(true);
	cannon->SetActorEnableCollision(false);
}