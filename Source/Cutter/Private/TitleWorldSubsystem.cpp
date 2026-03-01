// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UTitleWorldSubsystem::MoveToStage(FName stageName)
{
	UE_LOG(LogTemp, Log, TEXT("遷移するステージ: %s"), *stageName.ToString());
	UGameplayStatics::OpenLevel(GetWorld(), stageName);
}
