#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "TargetComponentBase.h"
#include "Obstacles/Cutters/Struct/CircleMoveCutterThrowTargetParam.h"
#include "LimitedRotateTargetComponent.generated.h"

/*
 * 回転範囲に制限があるターゲット用コンポーネント
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CUTTER_API ULimitedRotateTargetComponent : public UTargetComponentBase
{
	GENERATED_BODY()

public:
	ULimitedRotateTargetComponent();
	void RegisterParam(const FCircleMoveCutterThrowTargetParam& throwTargetParam);
	virtual void Init() override;

private:
	/*ターゲットのための回転*/
	virtual void Rotate(const FInputActionValue& Value) override;
	
	/*引数の値が指定値になっていたらカッターを投げる（入力検知入り登録用関数）*/
	virtual void Throw(const FInputActionValue& Value) override;
	
private:
	FCircleMoveCutterThrowTargetParam _throwTargetParam;
};
