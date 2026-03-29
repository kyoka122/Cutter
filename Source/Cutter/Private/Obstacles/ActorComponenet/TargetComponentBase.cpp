#include "TargetComponentBase.h"

#include "CutterCharacter.h"
#include "EnhancedInputComponent.h"
#include "Application/TagDefine.h"


void UTargetComponentBase::CacheArrowMesh()
{
	TArray<UStaticMeshComponent*> targetArrowMeshes;
	_owner = GetOwner();
	check(_owner);
	_owner->GetComponents<UStaticMeshComponent>(targetArrowMeshes);
	if (targetArrowMeshes.IsEmpty() || !IsValid(targetArrowMeshes[0]))
	{
		UE_LOG(LogTemp, Error, TEXT("キャラクターに矢印のスタティックメッシュが設定されていません。"));
		return;
	}
	TArray<UStaticMeshComponent*> throwTargetArrows = targetArrowMeshes.FilterByPredicate([](const UStaticMeshComponent* staticMesh)
	{
		return staticMesh->ComponentHasTag(TagDefine::ThrowArrow);
	});
	if (throwTargetArrows.Num() != 1)
	{
		UE_LOG(LogTemp, Error, TEXT("キャラクターに該当のタグがついたすStaticMeshを1つだけにしてください。StaticMesh数: %d個。　タグ名: %s"), throwTargetArrows.Num(), *TagDefine::ThrowArrow.ToString());
		return;
	}
	_throwArrowMesh = throwTargetArrows[0];
	UE_LOG(LogTemp, Log, TEXT("_throwArrowMesh: %s"), *_throwArrowMesh->GetName());
}

void UTargetComponentBase::VisibleArrowMesh()
{
	_throwArrowMesh->SetHiddenInGame(false);
}

void UTargetComponentBase::InVisibleArrowMesh()
{
	_throwArrowMesh->SetHiddenInGame(true);
}

void UTargetComponentBase::RegisterThrowable(AActor* throwable)
{
	_throwable = throwable;
}

void UTargetComponentBase::RegisterInputComponent()
{
	TObjectPtr<AActor> owner = GetOwner();
	check(owner);
	
	TObjectPtr<UInputAction> _throwAction;
	TObjectPtr<UInputAction> _moveAction;
	if (TObjectPtr<ACutterCharacter> cutterCharacter = Cast<ACutterCharacter>(owner))
	{
		_controller= cutterCharacter->Controller;
		_throwAction = cutterCharacter->ThrowAction;
		_moveAction = cutterCharacter->MoveAction;
		check(_controller);
		check(_throwAction);
		check(_moveAction);
		UE_LOG(LogTemp, Log, TEXT("RegisterInput"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("コンポーネント登録先のがアクタがACutterCharacterではありません"));
		return;
	}
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(owner->InputComponent))
	{
		EnhancedInputComponent->BindAction(_moveAction, ETriggerEvent::Triggered, this, &UTargetComponentBase::Move);
		EnhancedInputComponent->BindAction(_throwAction, ETriggerEvent::Started, this, &UTargetComponentBase::Throw);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void UTargetComponentBase::ReleaseInputComponent()
{
	AActor* owner = GetOwner();
	check(owner);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(owner->InputComponent))
	{
		EnhancedInputComponent->RemoveBindingByHandle(_bindingHandle);
		UE_LOG(LogTemp, Log, TEXT("_bindingHandle:"));		
		return;
	}
	UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
}