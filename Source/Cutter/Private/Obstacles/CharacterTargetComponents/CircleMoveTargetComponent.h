#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "TargetComponentBase.h"
#include "Obstacles/Cutters/Struct/CircleMoveCutterThrowTargetParam.h"
#include "CircleMoveTargetComponent.generated.h"

struct FCircleMoveCutterThrowParam;

/*
 * 回転範囲に制限があるターゲット用コンポーネント
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CUTTER_API UCircleMoveTargetComponent : public UTargetComponentBase
{
	GENERATED_BODY()

public:
	UCircleMoveTargetComponent();
	void RegisterParam(const FCircleMoveCutterThrowTargetParam& throwTargetParam, TFunction<void(const FCircleMoveCutterThrowParam&)>
	                   throwCutterFunc);
	virtual void Init() override;

private:
	/*ターゲットのための回転*/
	virtual void Rotate(const FInputActionValue& Value) override;
	
	/*引数の値が指定値になっていたらカッターを投げる（入力検知入り登録用関数）*/
	virtual void Throw(const FInputActionValue& Value) override;
	
private:
	FCircleMoveCutterThrowTargetParam _throwTargetParam;
	TFunction<void(const FCircleMoveCutterThrowParam&)> _throwCutterFunc = {};
};
