#include "CircleMoveTargetComponent.h"

#include "CutterCharacter.h"
#include "EnhancedInputComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "InGame/Stage/StageShape.h"
#include "InGame/Stage/Struct/IntersectionData.h"
#include "Struct/CircleMoveCutterThrowParam.h"

UCircleMoveTargetComponent::UCircleMoveTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCircleMoveTargetComponent::RegisterParam(const FCircleMoveCutterThrowTargetParam& throwTargetParam, TFunction<void(const FCircleMoveCutterThrowParam&)> throwCutterFunc)
{
	_throwTargetParam = throwTargetParam;
	_throwCutterFunc = throwCutterFunc;
}

void UCircleMoveTargetComponent::Init()
{
	Super::Init();
	RegisterInputComponent();
	VisibleArrowMesh();
	//TODO: 実装完了後削除
	
	FVector2D farPointOfTangency = IStageShape::Execute_GetFarPointOfTangency(
		_throwTargetParam.stageShape.GetObject(), );
	
	
	float rotateRadius = toStageCenterVec2D.Size();
	FVector2D rotateCenterPos2D = (pointOfTangency + _throwTargetParam.cutterPos) / 2;//円の端点2つ同士の中点から回転の中心点導出
	_rotateCenterPos = FVector(rotateCenterPos2D.X, rotateCenterPos2D.Y, IStageShape::Execute_GetCenterPos(_throwTargetParam.stageShape.GetObject()).Z);
	
	FRotator newActorRotator= FRotationMatrix::MakeFromX(FVector(_throwTargetParam.firstLookVec.X, _throwTargetParam.firstLookVec.Y, 0)).Rotator();
	_owner->SetActorRotation(newActorRotator);
	_owner->SetVisibilityMiniMap(true);
	_owner->UpdatePoints();
}

void UCircleMoveTargetComponent::Rotate(const FInputActionValue& Value)
{
	//TODO: 実装中
	FVector2D MovementVector = Value.Get<FVector2D>();
	
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
	
	const FRotator characterRotation = _controller->GetControlRotation();
	const FRotator characterYawRotation(0, characterRotation.Yaw, 0);
	const FVector characterForwardDirection = FRotationMatrix(characterYawRotation).GetUnitAxis(EAxis::X);
	FVector cameraToCharacterVec = _overViewCapture->GetComponentLocation() - _owner->GetActorLocation();
	FVector crossProduct = FVector::CrossProduct(cameraToCharacterVec, characterForwardDirection);
		
	//TODO: 実装完了後削除
	UE_LOG(LogTemp, Log, TEXT("------------------------"));
	UE_LOG(LogTemp, Log, TEXT("cameraToCharacterVec: %s"), *cameraToCharacterVec.ToString());
	UE_LOG(LogTemp, Log, TEXT("characterForwardDirection: %s"), *characterForwardDirection.ToString());
	UE_LOG(LogTemp, Log, TEXT("crossProduct: %s"), *crossProduct.ToString());
	UE_LOG(LogTemp, Log, TEXT("MovementVector.X: %f"), MovementVector.X);
	UE_LOG(LogTemp, Log, TEXT("------------------------"));
		
		
	if (crossProduct.Y > 0 && MovementVector.X < 0)
	{
		_owner->AddControllerYawInput(-MovementVector.X * _throwTargetParam.targetingRotateSpeed/100.f);
		//_owner->AddActorLocalRotation(FRotator(0,-MovementVector.X * _throwTargetParam.targetingRotateSpeed/100.f,0));
	}
	if (crossProduct.Y < 0 && MovementVector.X > 0)
	{
		_owner->AddControllerYawInput(MovementVector.X * _throwTargetParam.targetingRotateSpeed/100.f);
		//_owner->AddActorLocalRotation(FRotator(0,-MovementVector.X * _throwTargetParam.targetingRotateSpeed/100.f,0));
	}
	_owner->UpdatePoints();
}
	
void UCircleMoveTargetComponent::Throw(const FInputActionValue& Value)
{
	Super::Throw(Value);
	FCircleMoveCutterThrowParam param;
	//TODO: paramに値追加
	
	if (_throwCutterFunc)
	{
		_throwCutterFunc(param);
	}
	else UE_LOG(LogTemp, Error, TEXT("_throwFuncがnullです。 %s"), *GetName());
	
	ReleaseInputComponent();
	InVisibleArrowMesh();
	if (IsValid(_owner))
	{
		_owner->SetVisibilityMiniMap(false);
		_owner->OnThrow();
	}
	else UE_LOG(LogTemp, Error, TEXT("_ownerがnullです。 %s"), *GetName());	
	
	DestroyComponent();
}
