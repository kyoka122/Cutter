#include "RotateTargetComponent.h"

#include "CutterCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "InGame/Interface/Throwable.h"

URotateTargetComponent::URotateTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URotateTargetComponent::RegisterParam(const FCutterThrowTargetParam& throwTargetParam)
{
	_throwTargetParam = throwTargetParam;
}

void URotateTargetComponent::Init()
{
	Super::Init();
	RegisterInputComponent();
	VisibleArrowMesh();
	UE_LOG(LogTemp, Log, TEXT("_throwTargetParam.firstLookVec: %s"), *_throwTargetParam.firstLookVec.ToString());
	FRotator newActorRotator= FRotationMatrix::MakeFromX(FVector(_throwTargetParam.firstLookVec.X, _throwTargetParam.firstLookVec.Y, 0)).Rotator();
	_owner->SetActorRotation(newActorRotator);
	_owner->SetVisibilityMiniMap(true);
}

void URotateTargetComponent::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (_controller)
	{
		const FRotator characterRotation = _controller->GetControlRotation();
		const FRotator characterYawRotation(0, characterRotation.Yaw, 0);
		const FVector characterForwardDirection = FRotationMatrix(characterYawRotation).GetUnitAxis(EAxis::X);
		FVector cameraToCharacterVec = _followCamera->GetComponentLocation() - _owner->GetActorLocation();
		
		FVector crossProduct = FVector::CrossProduct(cameraToCharacterVec, characterForwardDirection);
		UE_LOG(LogTemp, Log, TEXT("------------------------"));
		UE_LOG(LogTemp, Log, TEXT("_followCamera: %s"), *_followCamera->GetComponentLocation().ToString());
		UE_LOG(LogTemp, Log, TEXT("cameraToCharacterVec: %s"), *cameraToCharacterVec.ToString());
		UE_LOG(LogTemp, Log, TEXT("characterForwardDirection: %s"), *characterForwardDirection.ToString());
		UE_LOG(LogTemp, Log, TEXT("crossProduct: %s"), *crossProduct.ToString());
		UE_LOG(LogTemp, Log, TEXT("MovementVector.X: %f"), MovementVector.X);
		UE_LOG(LogTemp, Log, TEXT("------------------------"));
		if (crossProduct.Y > 0 && MovementVector.X < 0)
		{
			_owner->AddControllerYawInput(-MovementVector.X * _throwTargetParam.targetingRotateSpeed/100.f);
			_owner->AddActorLocalRotation(FRotator(0,-MovementVector.X * _throwTargetParam.targetingRotateSpeed/100.f,0));
		}
		if (crossProduct.Y < 0 && MovementVector.X > 0)
		{
			_owner->AddActorLocalRotation(FRotator(0,-MovementVector.X * _throwTargetParam.targetingRotateSpeed/100.f,0));
		}
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
	InVisibleArrowMesh();
	_owner->SetVisibilityMiniMap(false);
	
	DestroyComponent();
}
