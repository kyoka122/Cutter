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
	VisibleArrowMesh();
	
	if (!IsValid(_owner))
	{
		UE_LOG(LogTemp, Error, TEXT("_ownerがnullです。 %s"), *GetName());
		return;
	}
	FVector2D pointOfTangency = IStageShape::Execute_GetFarPointOfTangency(_throwTargetParam.stageShape.GetObject(), _throwTargetParam.cutterPos);
	_toCenterVec = (pointOfTangency - _throwTargetParam.cutterPos) / 2;
	_initToCenterVec = _toCenterVec;
	
	float radius = _toCenterVec.Size();
	FVector2D centerPos = (pointOfTangency + _throwTargetParam.cutterPos) / 2;//円の端点2つ同士の中点から回転の中心点導出
	
	//θが現在の中心角。l(弧の長さ)が大きいほど拡縮が緩やかになる
	//rθ = l(弧の長さ)、(π - θ) / 2 = λ (現在地から中心へ向かうベクトルとl(弧)が成す2等辺三角形の内、θではない方)
	//λ = (π - l / r) / 2
	_circleLineDirectionAngle = (180 - _throwTargetParam.accuracy / radius) / 2;
	
	FRotator rotatorMatrix = FRotator(0, _circleLineDirectionAngle, 0);
	FVector newDirection = rotatorMatrix.RotateVector(FVector(_initToCenterVec.X, _initToCenterVec.Y, 0));
	_owner->SetActorRotation(newDirection.Rotation());
	_owner->SetVisibilityMiniMap(true);
	_owner->SetVisibleCutterLooksView(_throwTargetParam.looksTexture);
	
	UpdatePaints(centerPos, radius);
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
	
	_owner->AddActorLocalRotation(GetRotatorByInput(MovementVector));
	FRotator characterYawRotation = FRotator(0, _owner->GetActorRotation().Yaw, 0);
	FVector characterForwardDirection = FRotationMatrix(characterYawRotation).GetUnitAxis(EAxis::X);
	
	UpdateCircle(FVector2D(characterForwardDirection));
}

FRotator UCircleMoveTargetComponent::GetRotatorByInput(FVector2D input) const
{
	float characterRotateYaw = _owner->GetActorRotation().Yaw;
	FRotator characterYawRotation(0, _owner->GetActorRotation().Yaw, 0);
	FVector characterForwardDirection = FRotationMatrix(characterYawRotation).GetUnitAxis(EAxis::X);
	
	float value = input.Size() * _throwTargetParam.rotateSpeed;
	int32 direction = 0;
		
	if (FMath::Abs(input.X) > FMath::Abs(input.Y))//MEMO: X,Yの入力に対し、値の大きい方を優先して処理する
	{
		if (input.X > 0)
		{
			direction = characterForwardDirection.X >= 0 ? 1 : -1;
			value = FMath::Min(FMath::Abs(90.f - characterRotateYaw), value);//回転の境界値に来た場合、移動量の制限を行う
		} 
		else if (input.X < 0)
		{
			direction = characterForwardDirection.X <= 0 ? 1 : -1;
			value = FMath::Min(FMath::Abs(-90.f - characterRotateYaw), value);//回転の境界値に来た場合、移動量の制限を行う
		}
	}
	else
	{
		if (input.Y > 0)
		{
			direction = characterForwardDirection.Y <= 0 ? 1 : -1;
			value = FMath::Min(FMath::Abs(characterRotateYaw), value);//回転の境界値に来た場合、移動量の制限を行う
		}
		else if (input.Y < 0)
		{
			direction = characterForwardDirection.Y >= 0 ? 1 : -1;
			value = FMath::Min(FMath::Abs(180.f - characterRotateYaw), value);//回転の境界値に来た場合、移動量の制限を行う
			value = FMath::Min(FMath::Abs(-180.f - characterRotateYaw), value);//左向きの時のみ、ActorRotation.Yowが-か+かどちらか分からないので、両方計算。
		}
	}
	
	return FRotator(0.f, direction * value, 0.f);
}

void UCircleMoveTargetComponent::UpdateCircle(FVector2D direction)
{
	float circleLineDirectionCos = FVector2D::DotProduct(_initToCenterVec, direction) / (_initToCenterVec.Size() * direction.Size());//内積と辺の長さから角度割り出し
	int32 rotateDirection = FVector2D::CrossProduct(_initToCenterVec, direction) >= 0 ? 1 : -1;
	float circleLineDirectionRad = rotateDirection * FMath::Acos(circleLineDirectionCos);
	_circleLineDirectionAngle = circleLineDirectionRad * 180 / PI;//ラジアンから度に変換
	//θが現在の中心角の時、l(弧の長さ)が小さいほどより正確に接線を示すようになるため、lはキャラクターの向きの精度と言える
	//rθ = l(弧の長さ)、(π - θ) / 2 = λ (現在地から中心へ向かうベクトルとl(弧)が成す2等辺三角形の内、θではない方)
	//r = l/(π - 2 * λ)
	float radius = _throwTargetParam.accuracy/(180 - 2 * FMath::Abs(_circleLineDirectionAngle));
	_toCenterVec = _initToCenterVec.GetSafeNormal() * radius;
	
	FVector2D centerPos = _throwTargetParam.cutterPos + _toCenterVec.GetSafeNormal() * radius;//円の端点2つ同士の中点から回転の中心点導出
	UpdatePaints(centerPos, radius);
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
	param.toStageCenterVec2D = _toCenterVec;
	param.rotateDirection = _circleLineDirectionAngle >= 0 ? -1 : 1;
	
	if (_throwCutterFunc)
	{
		_throwCutterFunc(param);
	}
	else UE_LOG(LogTemp, Error, TEXT("_throwFuncがnullです。 %s"), *GetName());
	
	ReleaseInputComponent();
	InVisibleArrowMesh();
	if (IsValid(_owner))
	{
		_owner->SetVisibilityMiniMap(false);
		_owner->OnThrow();
	}
	else UE_LOG(LogTemp, Error, TEXT("_ownerがnullです。 %s"), *GetName());	
	
	DestroyComponent();
}
