#pragma once

#include "CutterBlowParam.generated.h"

class IStageShape;

/**
 *  カッターを吹き飛ばすアクタの外部入力パラメータ
 */
USTRUCT(BlueprintType)
struct FCutterBlowParam
{
	GENERATED_BODY()
	
public:
	/*移動方向*/
	FVector2D moveDirection = {};
	
	/*ステージ形状方法*/
	UPROPERTY() TScriptInterface<IStageShape> stageShape = {};
};
