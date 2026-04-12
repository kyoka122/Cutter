#include "FullRotateTargetComponent.h"

#include "CutterCharacter.h"
#include "InputActionValue.h"
#include "InGame/Interface/Throwable.h"

UFullRotateTargetComponent::UFullRotateTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFullRotateTargetComponent::RegisterParam(const FStraightYoYoThrowTargetParam& throwTargetParam)
{
	_throwTargetParam = throwTargetParam;
}

void UFullRotateTargetComponent::Init()
{
	Super::Init();
	RegisterInputComponent();
	VisibleArrowMesh();
	FRotator newActorRotator= FRotationMatrix::MakeFromX(FVector(_throwTargetParam.firstLookVec.X, _throwTargetParam.firstLookVec.Y, 0)).Rotator();
	_owner->SetActorRotation(newActorRotator);
	_owner->SetVisibilityMiniMap(true);
}

void UFullRotateTargetComponent::Rotate(const FInputActionValue& Value)
{
	//TODO: 実装中
	// FVector2D MovementVector = Value.Get<FVector2D>();
	//
	// if (_controller != nullptr)
	// {
	// 	const FRotator Rotation = _controller->GetControlRotation();
	// 	
	// 	const FRotator YawRotation(0, Rotation.Yaw, 0);
	// 	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// 	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// }
}
	
void UFullRotateTargetComponent::Throw(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Throw %s"), *GetName());
	Super::Throw(Value);
	if (_throwable.GetInterface())
	{
		if (ACutterCharacter* cutterCharacter = Cast<ACutterCharacter>(_owner))
		{
			cutterCharacter->OnThrow();
		}
		IThrowable::Execute_Throw(_throwable.GetObject());
	}
	ReleaseInputComponent();
	InVisibleArrowMesh();
	_owner->SetVisibilityMiniMap(false);
	
	DestroyComponent();
}
