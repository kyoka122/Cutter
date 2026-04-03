#include "TargetComponentBase.h"

#include "CutterCharacter.h"
#include "EnhancedInputComponent.h"
#include "Application/TagDefine.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"


void UTargetComponentBase::Init()
{
	CacheOwner();
	check(_owner)
	CacheArrowMesh();
	check(_throwArrowMesh)
	CacheCamera();
	check(_followCamera)
}

void UTargetComponentBase::CacheArrowMesh()
{
	TArray<UStaticMeshComponent*> targetArrowMeshes;
	
	//MEMO: コンポーネントの参照の取り方が分からないので全検索+タグ検索方式で。
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
	UE_LOG(LogTemp, Log, TEXT("_throwArrowMesh: %s"), *_throwArrowMesh->GetName());
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
		UE_LOG(LogTemp, Log, TEXT("コンポーネント登録先のがアクタがACutterCharacterではありません"));
	}
}

void UTargetComponentBase::CacheCamera()
{
	_followCamera = _owner->GetFollowCamera();
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
	check(_owner);

	_controller= _owner->Controller;
	UInputAction* _throwAction = _owner->ThrowAction;
	UInputAction* _moveAction = _owner->MoveAction;
	check(_controller);
	check(_throwAction);
	check(_moveAction);
	UE_LOG(LogTemp, Log, TEXT("RegisterInput"));
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(_owner->InputComponent))
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
	check(_owner);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(_owner->InputComponent))
	{
		EnhancedInputComponent->RemoveBindingByHandle(_bindingHandle);
		UE_LOG(LogTemp, Log, TEXT("_bindingHandle:"));		
		return;
	}
	UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
}

void UTargetComponentBase::MoveToOverViewCamera()
{
	ACameraActor* cameraActor = _owner->GetOverViewCamera();
	check(IsValid(cameraActor));
	UGameplayStatics::GetPlayerController(GetWorld(),0)->SetViewTargetWithBlend(cameraActor,1.0f);
}
