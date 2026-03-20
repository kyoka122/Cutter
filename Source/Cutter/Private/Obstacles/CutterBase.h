#pragma once

#include "CoreMinimal.h"
#include "DataAsset/CutterBaseParam.h"
#include "Enum/CutterMode.h"
#include "GameFramework/Actor.h"
#include "CutterBase.generated.h"

UCLASS()
class CUTTER_API ACutterBase : public AActor
{
	GENERATED_BODY()
	
public:
	ACutterBase();
	virtual void BeginPlay() override;
	virtual FCutterBaseParam GetBaseParam() const {return {};}
	virtual ECutterMode GetCurrentMode() const {return currentMode;}
	void RegisterAddScoreFunc(TFunction<void(int)> scoreAddFunc);
	
protected:
	typedef TFunction<void(AActor*)> OverlapFunc;
	TArray<OverlapFunc> _overlapFunc;
	
	//template <typename T = ABaseCutter>
	void RegisterEvent(UStaticMeshComponent* staticMeshComponent,OverlapFunc func)
	{
		check(IsValid(staticMeshComponent));
		_overlapFunc.Add(func);
		staticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ACutterBase::OnBeginOverlapEvent);
	}

protected:
	ECutterMode currentMode = {};
	
private:
	UFUNCTION()
	virtual void OnBeginOverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
	{
		if (_overlapFunc.IsEmpty())
		{
			UE_LOG(LogTemp, Log, TEXT("実行する関数がnullです:"));
		}
		for (auto& overlapFunc : _overlapFunc)
		{
			overlapFunc(OtherActor);
		}
	};
	
protected:
	TFunction<void(int)> _scoreAddFunc = {};
	// UFUNCTION(BlueprintCallable, Category = "InGame")
	// void OnOverlapScoreGetable(const AActor* otherActor){}
	//
	// UFUNCTION(BlueprintCallable, Category = "InGame")
	// void OnOverlapDamageableActor(const AActor* otherActor){}
	
	// UFUNCTION()
	// void TryBreak(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//
	// UFUNCTION()
	// void TryAddScore(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//
	// UFUNCTION()
	// void TryDamage(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//
};
