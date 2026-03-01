#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StageRowData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FStageRowData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName levelName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText text;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* image;
};
