#include "BambooSealed.h"

ABambooSealed::ABambooSealed()
{
	PrimaryActorTick.bCanEverTick = true;
}

int ABambooSealed::RobbedScore_Implementation(bool isExecPlayer)
{
	if (isExecPlayer)
	{
		return 0;
	}
	if (_transformCutterFunc)
	{
		//当たり判定消す
		_transformCutterFunc(_type, GetActorTransform());
		//演出実行
		return _score;
	}
	return 0;
}