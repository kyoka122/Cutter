#include "StraightYoYoTargetComponent.h"

#include "CutterCharacter.h"
#include "InputActionValue.h"
#include "InGame/Stage/StageShape.h"
#include "Struct/StraightYoYoThrowParam.h"

UStraightYoYoTargetComponent::UStraightYoYoTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStraightYoYoTargetComponent::RegisterParam(const FStraightYoYoThrowTargetParam& throwTargetParam, const TFunction<void(const FStraightYoYoThrowParam&)>& throwFunc)
{
	_throwTargetParam = throwTargetParam;
	_throwCutterFunc = throwFunc;
}

void UStraightYoYoTargetComponent::Init()
{
	Super::Init();
	RegisterInputComponent();
	VisibleArrowMesh();
	
	if (!IsValid(_owner))
	{
		UE_LOG(LogTemp, Error, TEXT("_ownerがnullです。 %s"), *GetName());
		return;
	}
	
	FVector2D currentTargetVec = _throwTargetParam.firstLookVec;
	FRotator newActorRotator = FRotationMatrix::MakeFromX(FVector(currentTargetVec.X, currentTargetVec.Y, 0)).Rotator();
	_owner->SetActorRotation(newActorRotator);
	_owner->SetVisibilityMiniMap(true);
	_owner->SetVisibleCutterLooksView(_throwTargetParam.looksTexture);
	
	FIntersectionData intersectionData = IStageShape::Execute_GetInterSections(
		_throwTargetParam.stageShape.GetObject(), _throwTargetParam.cutterPos, _throwTargetParam.firstLookVec);
	_owner->UpdatePoints({intersectionData.point1, intersectionData.point2});
}

void UStraightYoYoTargetComponent::Rotate(const FInputActionValue& Value)
{
	//TODO: 実装中
	FVector2D MovementVector = Value.Get<FVector2D>();//MEMO: W: +Y, A: -X, S: -Y, D: X
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
	
	float characterRotateYaw = _owner->GetActorRotation().Yaw;
	FRotator characterYawRotation(0, _owner->GetActorRotation().Yaw, 0);
	FVector characterForwardDirection = FRotationMatrix(characterYawRotation).GetUnitAxis(EAxis::X);
		
	int32 direction = 0;
	float value = Value.GetMagnitude() * _throwTargetParam.rotateSpeed;
	
	if (FMath::Abs(MovementVector.X) > FMath::Abs(MovementVector.Y))//MEMO: X,Yの入力に対し、値の大きい方を優先して処理する
	{
		if (MovementVector.X > 0)
		{
			direction = characterForwardDirection.X >= 0 ? 1 : -1;
			value = FMath::Min(FMath::Abs(90.f - characterRotateYaw), value);//回転の境界値に来た場合、移動量の制限を行う
		} 
		else if (MovementVector.X < 0)
		{
			direction = characterForwardDirection.X <= 0 ? 1 : -1;
			value = FMath::Min(FMath::Abs(-90.f - characterRotateYaw), value);//回転の境界値に来た場合、移動量の制限を行う
		}
	}
	else
	{
		if (MovementVector.Y > 0)
		{
			direction = characterForwardDirection.Y <= 0 ? 1 : -1;
			value = FMath::Min(FMath::Abs(characterRotateYaw), value);//回転の境界値に来た場合、移動量の制限を行う
		}
		else if (MovementVector.Y < 0)
		{
			direction = characterForwardDirection.Y >= 0 ? 1 : -1;
			value = FMath::Min(FMath::Abs(180.f - characterRotateYaw), value);//回転の境界値に来た場合、移動量の制限を行う
			value = FMath::Min(FMath::Abs(-180.f - characterRotateYaw), value);//左向きの時のみ、ActorRotation.Yowが-か+かどちらか分からないので、両方計算。
		}
	}
	//TODO: AddからSetRotationに直す
	_owner->AddActorLocalRotation(FRotator(0.f, direction * value, 0.f));
	
	characterYawRotation = FRotator(0, _owner->GetActorRotation().Yaw, 0);
	characterForwardDirection = FRotationMatrix(characterYawRotation).GetUnitAxis(EAxis::X);
	FIntersectionData intersectionData = IStageShape::Execute_GetInterSections(
		_throwTargetParam.stageShape.GetObject(), _throwTargetParam.cutterPos, FVector2D(characterForwardDirection));
	_owner->UpdatePoints({intersectionData.point1, intersectionData.point2});
}
	
void UStraightYoYoTargetComponent::Throw(const FInputActionValue& Value)
{
	FStraightYoYoThrowParam param;
	const FRotator characterYawRotation(0, _owner->GetActorRotation().Yaw, 0);
	const FVector currentTargetVec = FRotationMatrix(characterYawRotation).GetUnitAxis(EAxis::X);
	param.moveVec = FVector2D(currentTargetVec);
	
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
