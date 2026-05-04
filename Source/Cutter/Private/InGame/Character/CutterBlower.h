#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Struct/CutterBlowParam.h"
#include "CutterBlower.generated.h"

/**
 *  カッターを吹き飛ばすアクタ
 */
UCLASS()
class CUTTER_API ACutterBlower : public AActor
{
	GENERATED_BODY()

public:
	ACutterBlower();
	virtual void Tick(float DeltaTime) override;
	
	/*移動に必要なパラメータ(移動方向等)を設定する*/
	void RegisterMoveParam(const FCutterBlowParam& param);
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Character")
	UStaticMeshComponent* GetStaticMesh();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "移動速度")
	float moveSpeed = 200.f;
	
private:
	void Move(float deltaTime);

	UFUNCTION()
	void OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                         int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/*破壊可能なものかどうかをチェックして破壊する*/
	void OnOverlapBreakable(AActor* otherActor) const;
	
	/*自身を破壊する時に呼ばれる関数*/
	void OnBreak();
	
private:
	FCutterBlowParam _param;
	UPROPERTY() TObjectPtr<UStaticMeshComponent> _staticMeshComponent = {};
};
