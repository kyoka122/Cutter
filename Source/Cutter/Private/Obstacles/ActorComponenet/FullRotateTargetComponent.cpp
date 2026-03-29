#include "FullRotateTargetComponent.h"
#include "InputActionValue.h"
#include "InGame/Interface/Throwable.h"

UFullRotateTargetComponent::UFullRotateTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFullRotateTargetComponent::RegisterParam(const FCutterThrowTargetParam& throwTargetParam)
{
	_throwTargetParam = throwTargetParam;
}

void UFullRotateTargetComponent::Init()
{
	Super::BeginPlay();
	CacheArrowMesh();
	RegisterInputComponent();
	VisibleArrowMesh();
}

void UFullRotateTargetComponent::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (_controller != nullptr)
	{
		const FRotator Rotation = _controller->GetControlRotation();
		
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	}
}
	
void UFullRotateTargetComponent::Throw(const FInputActionValue& Value)
{
	Super::Throw(Value);
	if (_throwable.GetInterface())
	{
		_throwable->Throw();
	}
	ReleaseInputComponent();
	DestroyComponent();
}
