#pragma once

#include "CoreMinimal.h"
#include "Throwable.generated.h"

struct FCircleMoveCutterThrowTargetParam;

/**
 * 衝突により投げることができるアクタのインターフェース(UObject)
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UThrowable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 衝突により投げることができるアクタのインターフェース
 */
class CUTTER_API IThrowable
{
	GENERATED_BODY()
	
public:
	/*投げる際のターゲットを行う*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InGame")
	void StartTargeting(AActor* throwActor);
	virtual void StartTargeting_Implementation(AActor* throwActor) = 0;

	/*アクタの位置を取得する*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InGame")
	FVector GetLocation();
	virtual FVector GetLocation_Implementation() const = 0;
};