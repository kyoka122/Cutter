#include "CutterBase.h"
#include "InGame/Interface/Breakable.h"

ACutterBase::ACutterBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACutterBase::BeginPlay()
{
	Super::BeginPlay();
	currentMode = ECutterMode::Sphere;
}

void ACutterBase::RegisterAddScoreFunc(TFunction<void(int)> scoreAddFunc)
{
	_scoreAddFunc = scoreAddFunc;
}