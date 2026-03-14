// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/ObstacleBaseParam.h"
#include "GameFramework/Actor.h"
#include "BaseObstacle.generated.h"

UCLASS()
class CUTTER_API ABaseObstacle : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseObstacle();
	virtual FObstacleBaseParam GetBaseParam() const {return {};}
};
