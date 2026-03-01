// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TitleWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UTitleWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Stage")
	void MoveToStage(FName stageName);
};
