#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CircleExpandCutter.generated.h"

/**
 * 円形ステージを円を描くように回りながら、中心から徐々に外に向かって円が広がっていくカッタークラス
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CUTTER_API UCircleExpandCutter : public UActorComponent
{
	GENERATED_BODY()

public:
	UCircleExpandCutter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void Init();

private:
	void Translate(float DeltaTime);
	FVector CalcPosition(float DeltaTime);
	FQuat CalcRotation(float DeltaTime);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "回転パラメータ", meta = (AllowPrivateAccess = "true"))
	float _rotateRate = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "移動パラメータ", meta = (AllowPrivateAccess = "true"))
	float _moveRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "移動パラメータ", meta = (AllowPrivateAccess = "true"))
	FVector _rotateCenterPos = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "移動パラメータ", meta = (AllowPrivateAccess = "true"))
	float _pitch = 10.f;

	TObjectPtr<AActor> _ownerActor = {};
	float _currentAngle = 0.0f;
};
