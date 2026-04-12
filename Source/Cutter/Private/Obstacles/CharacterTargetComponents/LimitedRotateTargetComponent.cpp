#include "LimitedRotateTargetComponent.h"

#include "CutterCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "InGame/Interface/Throwable.h"

ULimitedRotateTargetComponent::ULimitedRotateTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULimitedRotateTargetComponent::RegisterParam(const FCircleMoveCutterThrowTargetParam& throwTargetParam)
{
	_throwTargetParam = throwTargetParam;
}

void ULimitedRotateTargetComponent::Init()
{
	Super::Init();
	RegisterInputComponent();
	VisibleArrowMesh();
	//TODO: 実装完了後削除
	UE_LOG(LogTemp, Log, TEXT("_throwTargetParam.firstLookVec: %s"), *_throwTargetParam.firstLookVec.ToString());
	FRotator newActorRotator= FRotationMatrix::MakeFromX(FVector(_throwTargetParam.firstLookVec.X, _throwTargetParam.firstLookVec.Y, 0)).Rotator();
	_owner->SetActorRotation(newActorRotator);
	_owner->SetVisibilityMiniMap(true);
}

void ULimitedRotateTargetComponent::Rotate(const FInputActionValue& Value)
{
	//TODO: 実装中
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	// if (_controller)
	// {
	// 	const FRotator characterRotation = _controller->GetControlRotation();
	// 	const FRotator characterYawRotation(0, characterRotation.Yaw, 0);
	// 	const FVector characterForwardDirection = FRotationMatrix(characterYawRotation).GetUnitAxis(EAxis::X);
	// 	FVector cameraToCharacterVec = _followCamera->GetComponentLocation() - _owner->GetActorLocation();
	// 	
	// 	FVector crossProduct = FVector::CrossProduct(cameraToCharacterVec, characterForwardDirection);
	// 	
	// 	//TODO: 実装完了後削除
	// 	UE_LOG(LogTemp, Log, TEXT("------------------------"));
	// 	UE_LOG(LogTemp, Log, TEXT("_followCamera: %s"), *_followCamera->GetComponentLocation().ToString());
	// 	UE_LOG(LogTemp, Log, TEXT("cameraToCharacterVec: %s"), *cameraToCharacterVec.ToString());
	// 	UE_LOG(LogTemp, Log, TEXT("characterForwardDirection: %s"), *characterForwardDirection.ToString());
	// 	UE_LOG(LogTemp, Log, TEXT("crossProduct: %s"), *crossProduct.ToString());
	// 	UE_LOG(LogTemp, Log, TEXT("MovementVector.X: %f"), MovementVector.X);
	// 	UE_LOG(LogTemp, Log, TEXT("------------------------"));
	// 	
	// 	
	// 	 if (crossProduct.Y > 0 && MovementVector.X < 0)
	// 	 {
	// 		_owner->AddControllerYawInput(-MovementVector.X * _throwTargetParam.targetingRotateSpeed/100.f);
	// 		_owner->AddActorLocalRotation(FRotator(0,-MovementVector.X * _throwTargetParam.targetingRotateSpeed/100.f,0));
	// 	 }
	// 	 if (crossProduct.Y < 0 && MovementVector.X > 0)
	// 	 {
	// 	 	_owner->AddActorLocalRotation(FRotator(0,-MovementVector.X * _throwTargetParam.targetingRotateSpeed/100.f,0));
	// 	 }
	// }
}
	
void ULimitedRotateTargetComponent::Throw(const FInputActionValue& Value)
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
