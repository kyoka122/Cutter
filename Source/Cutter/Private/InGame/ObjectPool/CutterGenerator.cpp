#include "CutterGenerator.h"

#include "Kismet/GameplayStatics.h"
#include "Obstacles/Cutters/CutterBase.h"

void ACutterGenerator::RegisterGeneratePrefab(TSubclassOf<ACutterBase> prefab)
{
	_prefab = prefab;
}

void ACutterGenerator::RegisterParam(const TFunction<void(int)>& scoreAddFunc, const TFunction<void(ACutterBase* cutter)>& releaseFunc, const TScriptInterface<IStageShape>& stageShape)
{
	_scoreAddFunc = scoreAddFunc;
	_releaseFunc = releaseFunc;
	_stageShape = stageShape;
}

ACutterBase* ACutterGenerator::Generate()
{
	check(_prefab);
	ACutterBase* cutter = GetWorld()->SpawnActorDeferred<ACutterBase>(_prefab, FTransform::Identity);
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

void ACutterGenerator::Activate(ACutterBase* cutter, const FTransform& transform)
{
	cutter->SetActorTransform(transform);
}

void ACutterGenerator::Deactivate(ACutterBase* cutter)
{
	cutter->SetActorTickEnabled(false);
	cutter->SetActorHiddenInGame(true);
	cutter->SetActorEnableCollision(false);
}