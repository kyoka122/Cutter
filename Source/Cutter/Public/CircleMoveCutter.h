#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CircleMoveCutter.generated.h"

/**
 * 円形ステージを円周上に周るカッタークラス
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUTTER_API UCircleMoveCutter : public UActorComponent
{
	GENERATED_BODY()

public:
	UCircleMoveCutter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void Init();
	void Translate(float DeltaTime);
	FVector CalcPosition(float DeltaTime);
	FQuat CalcRotation(float DeltaTime);


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "ステージパラメータ", meta = (AllowPrivateAccess = "true"))
	FVector _stageCenterPos = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "ステージパラメータ", meta = (AllowPrivateAccess = "true"))
	float _stageSize = 4000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "移動パラメータ", meta = (AllowPrivateAccess = "true"))
	float _moveRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "移動パラメータ", meta = (AllowPrivateAccess = "true"))
	FVector _rotateCenterPos = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "回転パラメータ", meta = (AllowPrivateAccess = "true"))
	float _rotateRate = 10.f;

	TObjectPtr<AActor> _ownerActor = nullptr;
	float _rotateRadius = 0.0f;
	float _currentAngle = 0.0f;
};