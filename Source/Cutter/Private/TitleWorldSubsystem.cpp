#include "TitleWorldSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UTitleWorldSubsystem::MoveToStage(FName stageName)
{
	if (stageName == NAME_None)
	{
		UE_LOG(LogTemp, Error, TEXT("ステージの名前がNoneの状態で渡されました"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("遷移するステージ: %s"), *stageName.ToString());
	UGameplayStatics::OpenLevel(GetWorld(), stageName);
}