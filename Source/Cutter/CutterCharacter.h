// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InGame/InGameMode.h"
#include "InGame/UIs/InGameUI.h"
#include "Logging/LogMacros.h"
#include "CutterCharacter.generated.h"

class IOverViewMiniMap;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ACutterCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
public:
	/** Throw Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowAction;
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
public:
	ACutterCharacter();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	
	void Jump(const FInputActionValue& Value);
	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
protected:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool IsPlayingThrowAnimation();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool IsPlayingThrowStartAnimation();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetIsEndThrowAnimation();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool IsPlayingDamageAnimation();

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnThrow();

	/*Minimapの情報を登録*/
	void RegisterMiniMap(const TScriptInterface<IOverViewMiniMap>& overViewMinimap, USceneCaptureComponent2D* overViewCapture);
	
	/*MinimapのVisible切り替え(背景含む)*/
	void SetVisibilityMiniMap(bool value) const;
	
	/*Minimapに描画する線情報の更新*/
	void UpdatePoints(const TArray<FVector2D>& points) const;
	
	/*Minimapを映しているカメラ(CaptureComponent)　の情報を取得*/
	USceneCaptureComponent2D* GetOverViewCapture() const;

protected:
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
private:
	UPROPERTY() TScriptInterface<IOverViewMiniMap> _overViewMinimap = {};
	UPROPERTY() TObjectPtr<USceneCaptureComponent2D> _overViewCapture = {};
};

