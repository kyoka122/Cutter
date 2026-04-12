#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Obstacles/Struct/CutterSetData.h"
#include "CutterListDataAsset.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class CUTTER_API UCutterListDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "プレハブ情報登録", meta=(Categories = "CutterType"))
	TArray<FCutterSetData> prefabs = {};
};


