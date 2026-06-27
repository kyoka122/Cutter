#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CannonListDataAsset.generated.h"

struct FCannonData;

/**
 * 大砲のデータ
 */
UCLASS()
class CUTTER_API UCannonListDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "プレハブ情報登録", meta=(Categories = "CannonType"))
	FCannonData prefab = {};
};
