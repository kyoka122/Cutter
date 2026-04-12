#pragma once

#include "StageProperty.generated.h"

class IStageShape;
class UCameraComponent;

/**
 * ステージ情報を公開するためのインターフェース(UObject)
 */
UINTERFACE(MinimalAPI,Blueprintable)
class UStageProperty : public UInterface
{
	GENERATED_BODY()
};

/**
 * ステージ情報を公開するためのインターフェース
 */
class CUTTER_API IStageProperty
{
	GENERATED_BODY()
	
public:
	/*ステージの形状情報を取得する*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InGame")
	TScriptInterface<IStageShape> GetStageShape();
};