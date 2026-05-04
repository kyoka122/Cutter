#include "TargetComponentBase.h"

#include "CutterCharacter.h"
#include "EnhancedInputComponent.h"
#include "Application/TagDefine.h"

void UTargetComponentBase::Init()
{
	CacheOwner();
	CacheOverViewCapture();
}

void UTargetComponentBase::CacheOwner()
{
	AActor* owner = GetOwner();
	if (TObjectPtr<ACutterCharacter> cutterCharacter = Cast<ACutterCharacter>(owner))
	{
		_owner = cutterCharacter;
	}
	else UE_LOG(LogTemp, Error, TEXT("コンポーネント登録先のがアクタがACutterCharacterではありません"));
}

void UTargetComponentBase::CacheOverViewCapture()
{
	_overViewCapture = _owner->GetOverViewCapture();
}

void UTargetComponentBase::RegisterInputComponent()
{
	if (!IsValid(_owner))
	{
		UE_LOG(LogTemp, Error, TEXT("_ownerがnullです。 %s"), *GetName());
		return;
	}
	
	_controller = _owner->Controller;
	UInputAction* throwAction = _owner->ThrowAction;
	UInputAction* moveAction = _owner->MoveAction;
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(_owner->InputComponent))
	{
		EnhancedInputComponent->BindAction(moveAction, ETriggerEvent::Started, this, &UTargetComponentBase::Rotate);
		EnhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &UTargetComponentBase::Rotate);
		EnhancedInputComponent->BindAction(throwAction, ETriggerEvent::Started, this, &UTargetComponentBase::Throw);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void UTargetComponentBase::ReleaseInputComponent() const
{
	if (!IsValid(_owner))
	{
		UE_LOG(LogTemp, Error, TEXT("_ownerがnullです。 %s"), *GetName());
		return;
	}
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(_owner->InputComponent))
	{
		EnhancedInputComponent->RemoveBindingByHandle(_bindingHandle);
		return;
	}
	UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
}
