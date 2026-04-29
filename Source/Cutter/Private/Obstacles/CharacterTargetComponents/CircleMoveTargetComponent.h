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
	void RegisterParam(const FCircleMoveCutterThrowTargetParam& throwTargetParam, const TFunction<void(const FCircleMoveCutterThrowParam&)>
	                   & throwCutterFunc);
	virtual void Init() override;

private:
	/*ターゲットのための回転*/
	virtual void Rotate(const FInputActionValue& Value) override;
	
	/*現在のキャラクターの向きを元に円サイズを更新*/
	void UpdateCircle(FVector2D direction);
	
	/*MiniMapへの描画内容の更新*/
	void UpdatePaints(FVector2D center, float radius) const;
	
	/*引数の値が指定値になっていたらカッターを投げる（入力検知入り登録用関数）*/
	virtual void Throw(const FInputActionValue& Value) override;
	
private:
	FCircleMoveCutterThrowTargetParam _throwTargetParam;
	TFunction<void(const FCircleMoveCutterThrowParam&)> _throwCutterFunc = {};
	
	/*現在地からステージ中央へのベクトルとの角度差分*/
	float _circleLineDirectionAngle = 0.f;
	
	/*現在地から円中央方向へのベクトル*/
	FVector2D _toCenterVec = {};
	
	/*現在地からステージ中央方向へのベクトル*/
	FVector2D _initToCenterVec = {};
};
