#pragma once

#include "CoreMinimal.h"
#include "TargetComponentBase.h"
#include "Components/ActorComponent.h"
#include "Obstacles/Cutters/Struct/StraightYoYoThrowTargetParam.h"
#include "FullRotateTargetComponent.generated.h"

/*
 * 360度回転できるターゲット用コンポーネント
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CUTTER_API UFullRotateTargetComponent : public UTargetComponentBase
{
	GENERATED_BODY()

public:
	UFullRotateTargetComponent();
	void RegisterParam(const FStraightYoYoThrowTargetParam& throwTargetParam);
	virtual void Init() override;

private:
	/*ターゲットのための回転*/
	virtual void Rotate(const FInputActionValue& Value) override;
	
	/*引数の値が指定値になっていたらカッターを投げる（入力検知入り登録用関数）*/
	virtual void Throw(const FInputActionValue& Value) override;
	
private:
	FStraightYoYoThrowTargetParam _throwTargetParam;
};
