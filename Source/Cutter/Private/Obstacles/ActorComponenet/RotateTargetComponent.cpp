#include "RotateTargetComponent.h"

#include "CutterCharacter.h"
#include "EnhancedInputComponent.h"
#include "InGame/Interface/Throwable.h"

URotateTargetComponent::URotateTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URotateTargetComponent::RegisterParam(const FCutterThrowTargetParam& throwTargetParam)
{
	_throwTargetParam = throwTargetParam;
	UE_LOG(LogTemp, Log, TEXT("RefisterParam"));
}

void URotateTargetComponent::Init()
{
	CacheArrowMesh();
	RegisterInputComponent();
	VisibleArrowMesh();
}

void URotateTargetComponent::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (_controller)
	{
		const FRotator Rotation = _controller->GetControlRotation();
		
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	}
}
	
void URotateTargetComponent::Throw(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Throw"));
	Super::Throw(Value);
	if (_throwable.GetInterface())
	{
		if (ACutterCharacter* cutterCharacter = Cast<ACutterCharacter>(_owner))
		{
			cutterCharacter->OnThrow();
		}
		_throwable->Throw_Implementation();
	}
	ReleaseInputComponent();
	DestroyComponent();
	InVisibleArrowMesh();
}
