#include "SealedBase.h"

ASealedBase::ASealedBase()
{

}

void ASealedBase::InitCutterSpawnData(int score, FGameplayTag type, CutterSpawnFunc cutterSpawnFunc)
{
	_score = score;
	_type= type;
	_cutterSpawnFunc = cutterSpawnFunc;
}

