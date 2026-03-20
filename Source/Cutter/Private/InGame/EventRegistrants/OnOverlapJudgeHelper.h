#pragma once

#include "InGame/EventRegistrants/OnBeginOverlapEventRegistrantBase.h"
#include "Obstacles/CutterBase.h"

class OnOverlapJudgeHelper
{

public:
	static void OnOverlapBreakable(ACutterBase* myActor, AActor* otherActor);
};
