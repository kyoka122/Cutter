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
	if (_cutterSpawnFunc)
	{
		//当たり判定消す
		_cutterSpawnFunc(_type, GetActorTransform());
		//演出実行
		return _score;
	}
	return 0;
}
//非同期実行？
void Destroy_Implemention()
{
	
}