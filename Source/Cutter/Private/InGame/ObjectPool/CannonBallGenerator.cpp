#include "CannonBallGenerator.h"

#include "Kismet/GameplayStatics.h"
#include "Obstacles/Cannon/CannonBall.h"

void ACannonBallGenerator::RegisterGeneratePrefab(TSubclassOf<ACannonBall> prefab)
{
	_prefab = prefab;
}

void ACannonBallGenerator::RegisterParam(const TFunction<void(int)>& scoreAddFunc, const TFunction<void(ACannonBall* cannonBall)>& releaseFunc
	, const TScriptInterface<IActorTransform>& playerTransform)
{
	check(playerTransform)
	_scoreAddFunc = scoreAddFunc;
	_releaseFunc = releaseFunc;
	_playerTransform = playerTransform;
}

ACannonBall* ACannonBallGenerator::Generate()
{
	ACannonBall* cannonBall = GetWorld()->SpawnActorDeferred<ACannonBall>(_prefab, FTransform::Identity);
	if (IsValid(cannonBall))
	{
		Deactivate(cannonBall);
		UGameplayStatics::FinishSpawningActor(cannonBall, FTransform::Identity);
		cannonBall->SetActorTickEnabled(false);//MEMO: Spawn後にTickが始まってしまうので、再度OFF
		cannonBall->RegisterReleaseFunc(_releaseFunc);
		cannonBall->RegisterPlayerLocation(_playerTransform);
		return cannonBall;
	}
	UE_LOG(LogTemp, Error, TEXT("オブジェクトを生成できませんでした。 Generator: cannonGenerator"));
	return nullptr;
}

void ACannonBallGenerator::Activate(ACannonBall* cannonBall, const FTransform& transform)
{
	cannonBall->SetActorTransform(transform);
}

void ACannonBallGenerator::Deactivate(ACannonBall* cannonBall)
{
	cannonBall->SetActorTickEnabled(false);
	cannonBall->SetActorHiddenInGame(true);
	cannonBall->SetActorEnableCollision(false);
}