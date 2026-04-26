#include "TargetComponentBase.h"

#include "CutterCharacter.h"
#include "EnhancedInputComponent.h"
#include "Application/TagDefine.h"

void UTargetComponentBase::Init()
{
	CacheOwner();
	check(_owner)
	CacheArrowMesh();
	check(_throwArrowMesh)
	CacheOverViewCapture();
	check(_overViewCapture)
}

void UTargetComponentBase::CacheArrowMesh()
{
	TArray<UStaticMeshComponent*> targetArrowMeshes;
	
	//MEMO: コンポーネントのBPでの参照の取り方が分からないので全検索+タグ検索方式で。
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
		UE_LOG(LogTemp, Error, TEXT("キャラクターに該当のタグがついたStaticMeshを1つだけにしてください。StaticMesh数: %d個。　タグ名: %s"), throwTargetArrows.Num(), *TagDefine::ThrowArrow.ToString());
		return;
	}
	_throwArrowMesh = throwTargetArrows[0];
}

void UTargetComponentBase::CacheOwner()
{
	AActor* owner = GetOwner();
	check(owner);
	if (TObjectPtr<ACutterCharacter> cutterCharacter = Cast<ACutterCharacter>(owner))
	{
		_owner = cutterCharacter;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("コンポーネント登録先のがアクタがACutterCharacterではありません"));
	}
}

void UTargetComponentBase::CacheOverViewCapture()
{
	_overViewCapture = _owner->GetOverViewCapture();
}

void UTargetComponentBase::VisibleArrowMesh() const
{
	_throwArrowMesh->SetHiddenInGame(false);
}

void UTargetComponentBase::InVisibleArrowMesh() const
{
	_throwArrowMesh->SetHiddenInGame(true);
}

void UTargetComponentBase::RegisterInputComponent()
{
	if (!IsValid(_owner))
	{
		UE_LOG(LogTemp, Error, TEXT("_ownerがnullです。 %s"), *GetName());
		return;
	}
	
	_controller = _owner->Controller;
	UInputAction* _throwAction = _owner->ThrowAction;
	UInputAction* _moveAction = _owner->MoveAction;//TODO: nullチェック
	check(_throwAction);
	check(_moveAction);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(_owner->InputComponent))
	{
		EnhancedInputComponent->BindAction(_moveAction, ETriggerEvent::Triggered, this, &UTargetComponentBase::Rotate);
		EnhancedInputComponent->BindAction(_throwAction, ETriggerEvent::Started, this, &UTargetComponentBase::Throw);
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
