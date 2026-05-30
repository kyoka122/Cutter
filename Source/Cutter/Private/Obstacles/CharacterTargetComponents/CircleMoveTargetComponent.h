#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "TargetComponentBase.h"
#include "Obstacles/Cutters/Struct/CircleMoveCutterThrowTargetParam.h"
#include "CircleMoveTargetComponent.generated.h"

struct FCircleMoveThrowParam;

/*
 * 回転範囲に制限があるターゲット用コンポーネント
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CUTTER_API UCircleMoveTargetComponent : public UTargetComponentBase
{
	GENERATED_BODY()

public:
	UCircleMoveTargetComponent();
	void RegisterParam(const FCircleMoveCutterThrowTargetParam& throwTargetParam,
		const TFunction<void(const FCircleMoveThrowParam&)>& throwCutterFunc);
	virtual void Init() override;
	
private:
	/*ターゲットのための回転*/
	virtual void Rotate(const FInputActionValue& Value) override;
	
	/*入力開始時に回転方向を設定する*/
	void SetRotateDirectionByInput(FVector2D inputVec, const FVector& characterForwardDirection);

	/*入力値から回転情報を取得*/
	FRotator GetRotatorByInput(FVector2D inputVec, const FVector& characterForwardDirection) const;

	/*現在のキャラクターの向きを元に円サイズを更新*/
	void UpdateCircle(FVector2D direction);
	
	/*MiniMapへの描画内容の更新*/
	void UpdatePaints(FVector2D center, float radius) const;
	
	/*引数の値が指定値になっていたらカッターを投げる（入力検知入り登録用関数）*/
	virtual void Throw(const FInputActionValue& Value) override;
	
private:
	FCircleMoveCutterThrowTargetParam _throwTargetParam;
	TFunction<void(const FCircleMoveThrowParam&)> _throwCutterFunc = {};
	
	
	float _currentCircleLineRadius = 0.f;
	
	/*円を最大の大きさで描いた時の半径*/
	float _maxRadius = 0.f;
	
	int32 _rotateDirection = 0;
	
	/*初期位置から円中央方向へのベクトル*/
	FVector2D _initToCenterVec = {};
	
	/*移動限界ベクトル(右)*/
	FVector _rightMaxVec = {};
	
	/*移動限界ベクトル(左)*/
	FVector _leftMaxVec = {};
	
	/*現在の回転方向が収束するベクトル(+の回転方向なら1, -の回転方向なら-1)*/
	int32 _currentRotateDirection = {};
	
	/*現在の回転方向が収束するベクトル(+の回転方向なら1, -の回転方向なら-1)*/
	FVector2D _currentInputDirection = {};
};
