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
	_owner->UpdateLine({intersectionData.point1, intersectionData.point2});
}

void UStraightYoYoTargetComponent::Rotate(const FInputActionValue& Value)
{
	FVector2D inputVec = Value.Get<FVector2D>();//MEMO: W: +Y, A: -X, S: -Y, D: X
	if (inputVec == FVector2D::Zero())
	{
		return;
	}
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
		
	
	SetRotateDirectionByInput(inputVec, characterForwardDirection);
	_owner->SetActorRotation(GetRotatorByInput(inputVec, characterForwardDirection));
	
	characterYawRotation = FRotator(0, _owner->GetActorRotation().Yaw, 0);
	characterForwardDirection = FRotationMatrix(characterYawRotation).GetUnitAxis(EAxis::X);
	FIntersectionData intersectionData = IStageShape::Execute_GetInterSections(
		_throwTargetParam.stageShape.GetObject(), _throwTargetParam.cutterPos, FVector2D(characterForwardDirection));
	_owner->UpdateLine({intersectionData.point1, intersectionData.point2});
}

void UStraightYoYoTargetComponent::SetRotateDirectionByInput(FVector2D inputVec, const FVector& characterForwardDirection)
{
	if (inputVec == FVector2D::Zero())
	{
		currentRotateDirection = 0;
		return;
	}
	int nextDirection;
	FVector2D nextInputDirection;
	if (FMath::Abs(inputVec.X) >= FMath::Abs(inputVec.Y))//MEMO: X,Yの入力に対し、値の大きい方を優先して処理する
	{
		nextInputDirection = FVector2D(inputVec.X, 0).GetSafeNormal();
		if (inputVec.X > 0)
		{
			nextDirection = characterForwardDirection.X >= 0 ? 1 : -1;
		} 
		else
		{
			nextDirection = characterForwardDirection.X <= 0 ? 1 : -1;
		}
	}
	else
	{
		nextInputDirection = FVector2D(0, inputVec.Y).GetSafeNormal();
		if (inputVec.Y > 0)
		{
			nextDirection = characterForwardDirection.Y <= 0 ? 1 : -1;
		}
		else
		{
			nextDirection = characterForwardDirection.Y >= 0 ? 1 : -1;
		}
	}
	//MEMO: 入力最大値が変わらない場合、回転方向は変えない
	if (nextInputDirection == _currentInputDirection)
	{
		return;
	}
	currentRotateDirection = nextDirection;
	_currentInputDirection = nextInputDirection;
}

FRotator UStraightYoYoTargetComponent::GetRotatorByInput(FVector2D inputVec, const FVector& characterForwardDirection) const
{
	if (inputVec == FVector2D::Zero())
	{
		return characterForwardDirection.Rotation();
	}
	float value = inputVec.Size() * _throwTargetParam.rotateSpeed;
	FVector rotatedVec = FRotator(0.f, currentRotateDirection * value, 0.f).RotateVector(characterForwardDirection);
	return rotatedVec.Rotation();
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
	if (IsValid(_owner))
	{
		_owner->SetVisibilityMiniMap(false);
		_owner->OnThrow();
	}
	else UE_LOG(LogTemp, Error, TEXT("_ownerがnullです。 %s"), *GetName());
	
	DestroyComponent();
}
