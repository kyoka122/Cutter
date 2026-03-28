#include "SealedBase.h"

ASealedBase::ASealedBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASealedBase::RegisterTransformCutterData(FGameplayTag type, const TransformCutterFunc& transformCutterFunc)
{
	_type = type;
	_transformCutterFunc = transformCutterFunc;
}

void ASealedBase::RegisterInactiveFunc(TFunction<void()> inactiveFunc)
{
	_inactiveFunc = inactiveFunc;
}