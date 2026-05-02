#pragma once

#include "CutterLooksView.generated.h"

struct FGameplayTag;
/*
 * カッターの見た目表示用インターフェース(UObject)
 */
UINTERFACE(MinimalAPI,Blueprintable)
class UCutterLooksView : public UInterface
{
	GENERATED_BODY()
};

/*
 * Cutterのイメージ表示ができるインターフェース
 */
class CUTTER_API ICutterLooksView
{
	GENERATED_BODY()
	
public:
	/*カッターイメージの表示表示非表示を切り替える*/
	virtual void SetVisibilityCutterLooksView(bool value) = 0;
	
	/*Cutter画像をセットする*/
	virtual void SetCutterImage(UTexture2D* texture) = 0;
};