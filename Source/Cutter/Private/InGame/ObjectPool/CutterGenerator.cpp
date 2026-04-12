#include "CutterGenerator.h"
#include "Kismet/GameplayStatics.h"
#include "Obstacles/CutterBase.h"

void ACutterGenerator::RegisterGeneratePrefab(TSubclassOf<ACutterBase> prefab)
{
	_prefab = prefab;
}

void ACutterGenerator::RegisterParam(TFunction<void(int)> scoreAddFunc, TFunction<void(ACutterBase* cutter)> releaseFunc, TScriptInterface<IStageShape> stageShape)
{
	_scoreAddFunc = scoreAddFunc;
	_releaseFunc = releaseFunc;
	_stageShape = stageShape;
}

TObjectPtr<ACutterBase> ACutterGenerator::Generate()
{
	check(_prefab);
	TObjectPtr<ACutterBase> cutter = GetWorld()->SpawnActorDeferred<ACutterBase>(_prefab, FTransform::Identity);
	if (IsValid(cutter))
	{
		Deactivate(cutter);
		UGameplayStatics::FinishSpawningActor(cutter, FTransform::Identity);
		cutter->SetActorTickEnabled(false);//MEMO: Spawn後にTickが始まってしまうので、再度OFF
		cutter->RegisterScoreAddFunc(_scoreAddFunc);
		cutter->RegisterStageShapeData(_stageShape);
		cutter->RegisterReleaseFunc(_releaseFunc);
		return cutter;
	}
	UE_LOG(LogTemp, Log, TEXT("オブジェクトを生成できませんでした。 Generator: CutterGenerator"));
	return nullptr;
}

void ACutterGenerator::Activate(TObjectPtr<ACutterBase> cutter, FTransform transform)
{
	cutter->SetActorTransform(transform);
}

void ACutterGenerator::Deactivate(TObjectPtr<ACutterBase> cutter)
{
	cutter->SetActorTickEnabled(false);
	cutter->SetActorHiddenInGame(true);
	cutter->SetActorEnableCollision(false);
}