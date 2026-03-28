#pragma once
#include "StageEnvironmentParam.generated.h"

USTRUCT(BlueprintType)
struct FStageEnvironmentParam : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName levelName = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float stageSize = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector centerPos = {};
};
