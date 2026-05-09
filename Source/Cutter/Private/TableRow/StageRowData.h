#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StageRowData.generated.h"

/**
 * ステージ情報登録用テーブル。主にレベル名や外観Imageなどのステージ選択画面で表示する基本情報を登録する。
 */
USTRUCT(BlueprintType)
struct FStageRowData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName levelName = {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText text = {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> image = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float limitTime = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float blowerCount = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> obstacleSpawnData = {};
};
