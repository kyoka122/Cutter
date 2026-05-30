#include "CircleMoveTargetComponent.h"

#include "CutterCharacter.h"
#include "EnhancedInputComponent.h"
#include "InGame/Stage/StageShape.h"
#include "Struct/CircleMoveThrowParam.h"

UCircleMoveTargetComponent::UCircleMoveTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCircleMoveTargetComponent::RegisterParam(const FCircleMoveCutterThrowTargetParam& throwTargetParam, const TFunction<void(const FCircleMoveThrowParam&)>& throwCutterFunc)
{
	_throwTargetParam = throwTargetParam;
	_throwCutterFunc = throwCutterFunc;
}

void UCircleMoveTargetComponent::Init()
{
	Super::Init();
	RegisterInputComponent();
	
	if (!IsValid(_owner))
	{
		UE_LOG(LogTemp, Error, TEXT("_ownerがnullです。 %s"), *GetName());
		return;
	}
	//内接円の最大サイズを求める（円の基本情報の取得も）
	FVector2D circleCenterPos = IStageShape::Execute_GetMaxSizeCircleCenterPos(_throwTargetParam.stageShape.GetObject(), _throwTargetParam.cutterPos);
	FVector2D toCenterVec = circleCenterPos - _throwTargetParam.cutterPos;
	_initToCenterVec = toCenterVec;
	_maxRadius = toCenterVec.Size();
	_currentCircleLineRadius = _maxRadius;
	
	//プレイヤーの回転角度の最大値を左右それぞれで求める
	FRotator leftRotator = FRotator(0, 90, 0);
	FVector leftMaxDirection = leftRotator.RotateVector(FVector(_initToCenterVec.X, _initToCenterVec.Y, 0));
	_leftMaxVec = leftMaxDirection;
	
	FRotator rightRotator = FRotator(0, -90, 0);
	FVector rightMaxDirection = rightRotator.RotateVector(FVector(_initToCenterVec.X, _initToCenterVec.Y, 0));
	_rightMaxVec = rightMaxDirection;
	
	//プレイヤーの向きを左の最大角にセット
	_owner->SetActorRotation(leftMaxDirection.Rotation());
	_owner->SetVisibilityMiniMap(true);
	_owner->SetVisibleCutterLooksView(_throwTargetParam.looksTexture);
	
	UpdatePaints(circleCenterPos, _currentCircleLineRadius);
}

void UCircleMoveTargetComponent::Rotate(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (!IsValid(_owner))
	{
		UE_LOG(LogTemp, Error, TEXT("_ownerがnullです。 %s"), *GetName());
		return;
	}
	if (!IsValid(_controller))
	{
		UE_LOG(LogTemp, Error, TEXT("_controllerがnullです。 %s"), *GetName());
		return;
	}
	
	FRotator characterYawRotation(0, _owner->GetActorRotation().Yaw, 0);
	FVector characterForwardDirection = FRotationMatrix(characterYawRotation).GetUnitAxis(EAxis::X);
	
	SetRotateDirectionByInput(MovementVector, characterForwardDirection);
	_owner->SetActorRotation(GetRotatorByInput(MovementVector, characterForwardDirection));
	characterYawRotation = FRotator(0, _owner->GetActorRotation().Yaw, 0);
	characterForwardDirection = FRotationMatrix(characterYawRotation).GetUnitAxis(EAxis::X);
	
	UpdateCircle(FVector2D(characterForwardDirection));
}

void UCircleMoveTargetComponent::SetRotateDirectionByInput(FVector2D inputVec, const FVector& characterForwardDirection)
{
	if (inputVec == FVector2D::Zero())
	{
		_currentRotateDirection = 0;
		_currentInputDirection = FVector2D::Zero();
		return;
	}
	
	FVector currentRotateConvergence;
	FVector2D nextInputDirection;
	
	if (FMath::Abs(inputVec.X) >= FMath::Abs(inputVec.Y))//MEMO: X,Yの入力に対し、値の大きい方を優先して処理する
	{
		nextInputDirection = FVector2D(inputVec.X, 0).GetSafeNormal();
		currentRotateConvergence = inputVec.X > 0 ? FVector::RightVector : FVector::LeftVector;
	}else
	{
		nextInputDirection = FVector2D(0, inputVec.Y).GetSafeNormal();
		currentRotateConvergence = inputVec.Y > 0 ? FVector::ForwardVector : FVector::BackwardVector;
	}
	
	//MEMO: 入力最大値が変わらない場合、回転方向は変えない
	if (_currentInputDirection == nextInputDirection)
	{
		return;
	}
	float crossProductZ = FVector::CrossProduct(currentRotateConvergence, characterForwardDirection).Z;
	_currentRotateDirection = crossProductZ >= 0 ? -1 : 1;
	_currentInputDirection = nextInputDirection;
}

FRotator UCircleMoveTargetComponent::GetRotatorByInput(FVector2D inputVec, const FVector& characterForwardDirection) const
{
	if (inputVec == FVector2D::Zero())
	{
		return characterForwardDirection.Rotation();
	}
	
	//1.回転目標の向きを計算
	float value = (inputVec * _currentInputDirection).Size() * _throwTargetParam.rotateSpeed;
	FVector rotatedVector = FRotator(0, FMath::Abs(value) * _currentRotateDirection,0).RotateVector(characterForwardDirection);
	
	//2.回転の最大値に応じて、移動量の制限を行う
	if (_currentRotateDirection == 1)
	{
		float rotatedCrossProductZ = FVector::CrossProduct(_leftMaxVec, rotatedVector).Z;
		rotatedVector = (rotatedCrossProductZ >= 0 ? -1 : 1) != _currentRotateDirection ? _leftMaxVec : rotatedVector;
	}
	else if (_currentRotateDirection == -1)
	{
		float rotatedCrossProductZ = FVector::CrossProduct(_rightMaxVec, rotatedVector).Z;
		rotatedVector = (rotatedCrossProductZ >= 0 ? -1 : 1) != _currentRotateDirection ? _rightMaxVec : rotatedVector;
	}
    	
	return rotatedVector.Rotation();
}

void UCircleMoveTargetComponent::UpdateCircle(FVector2D direction)
{
	float circleLineDirectionCos = FVector2D::DotProduct(_initToCenterVec, direction) / (_initToCenterVec.Size() * direction.Size());//内積と辺の長さから角度割り出し
	_rotateDirection = FVector2D::CrossProduct(_initToCenterVec, direction) >= 0 ? 1 : -1;
	float circleLineDirectionAngle = FMath::Acos(circleLineDirectionCos) * 180 / PI;//cos値から角度を求める (ラジアンから度に変換)

	//MEMO: 最大半径 / 90 * 現在の角度 =>回転割合(0 ~ 90度における)
	_currentCircleLineRadius = _maxRadius / 90 * circleLineDirectionAngle;
	FVector2D toCenterVec = _initToCenterVec.GetSafeNormal() * _currentCircleLineRadius;
	
	FVector2D centerPos = _throwTargetParam.cutterPos + toCenterVec;//円の端点2つ同士の中点から回転の中心点導出
	UpdatePaints(centerPos, _currentCircleLineRadius);
}

void UCircleMoveTargetComponent::UpdatePaints(FVector2D center, float radius) const
{
	TArray<FVector2D> points;
	int segments = _throwTargetParam.segments;
	points.Reserve(segments);
	for (int i = 0; i <= segments; ++i)
	{
		float radian = i * (2 * PI /segments);
		FVector2D vec(FMath::Cos(radian), FMath::Sin(radian));
		points.Add(center + vec * radius);
	}
	_owner->UpdatePoints(points);
}

void UCircleMoveTargetComponent::Throw(const FInputActionValue& Value)
{
	Super::Throw(Value);
	FCircleMoveThrowParam param;
	param.toStageCenterVec2D = _initToCenterVec.GetSafeNormal() * _currentCircleLineRadius;
	param.rotateDirection = _rotateDirection >= 0 ? -1 : 1;
	if (_throwCutterFunc)
	{
		_throwCutterFunc(param);
	}
	else UE_LOG(LogTemp, Error, TEXT("_throwFuncがnullです。 %s"), *GetName());
	
	ReleaseInputComponent();
	if (IsValid(_owner))
	{
		_owner->SetVisibilityMiniMap(false);
		_owner->OnThrow();
	}
	else UE_LOG(LogTemp, Error, TEXT("_ownerがnullです。 %s"), *GetName());	
	
	DestroyComponent();
}
