#include "InGame/UIs/InGameUI.h"

#include "MinimapUI.h"
#include "Application/CutterFormat.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/TextBlock.h"
#include "InGame/InGameState.h"

class UScrollBoxSlot;

void UInGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetScore(0, 0);
	SetTime(0);
}

void UInGameUI::UpdateUI(const AInGameState* gameState, float deltaTime)
{
	SetTime(gameState->GetLimitTime());
	SetBlowerCount(gameState->GetBlowerCount());
	if(_countAnimatedScore != gameState->GetScore())
	{
		SetScore(gameState->GetScore(), deltaTime);
	}
}

void UInGameUI::SetBlowerCount(int32 blowerCount)
{
	ESlateVisibility textVisibilityType = blowerCount > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	_blowerCountText->SetVisibility(textVisibilityType);
	_blowerCountText->SetText(FText::AsNumber(blowerCount));
	
	if (_currentSpawnedBlowerCount >= blowerCount)
	{
		int32 count = 0;
		for (const auto& blowerImage : _blowerImages)
		{
			ESlateVisibility imageVisibilityType = count < blowerCount ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
			blowerImage->SetVisibility(imageVisibilityType);
			count++;
		}
		return;
	}
	for (int i = _currentSpawnedBlowerCount; i < blowerCount; ++i)
	{
		TObjectPtr<UUserWidget> image = CreateWidget<UUserWidget>(this, _blowerImage);
		if (!image)
		{
			UE_LOG(LogTemp, Error, TEXT("Widget作成に失敗"));
			continue;
		}
		_blowerImages.Add(image);
		TObjectPtr<UOverlaySlot> overlaySlot = _blowerImagesOverlay->AddChildToOverlay(image);
		if (IsValid(overlaySlot))
		{
			overlaySlot->SetPadding(FMargin(i*50,0,0,0));//MEMO: もし大量に生成するようになってきたらPaddingの計算を行う
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("scrollBoxSlotの生成に失敗しました。 %s"), *GetName());
			return;
		}
	}
	_currentSpawnedBlowerCount = blowerCount;
}

void UInGameUI::SetVisibilityMiniMap(bool value)
{
	if (_miniMapOverlay)
	{
		ESlateVisibility visible = value ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		_miniMapOverlay->SetVisibility(visible);
	}
	else UE_LOG(LogTemp, Error, TEXT("_miniMapOverlayが見つかりませんでした。 %s"), *GetName());
	
	if (_miniMap)
	{
		_miniMap->SetParentVisible(value);
	}
	else UE_LOG(LogTemp, Error, TEXT("_miniMapが見つかりませんでした。 %s"), *GetName());
}

void UInGameUI::UpdateLine(const TArray<FVector2D>& linePoints, FVector cameraPos, float cameraOrthoWidth)
{
	if (_miniMap)
	{
		_miniMap->UpdateDrawLines(linePoints, cameraPos, cameraOrthoWidth);
	}
	else UE_LOG(LogTemp, Error, TEXT("_miniMapが見つかりませんでした。"));
}

void UInGameUI::UpdateBambooIcon(const TArray<FVector2D>& bambooIconPoints, FVector cameraPos, float cameraOrthoWidth)
{
	if (_miniMap)
	{
		_miniMap->UpdateBambooPoints(bambooIconPoints, cameraPos, cameraOrthoWidth);
	}
	else UE_LOG(LogTemp, Error, TEXT("_miniMapが見つかりませんでした。"));
}

void UInGameUI::SetVisibilityCutterLooksView(bool value)
{
	if (_cutterLooksOverlay)
	{
		ESlateVisibility visible = value ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		_cutterLooksOverlay->SetVisibility(visible);
	}
	else UE_LOG(LogTemp, Error, TEXT("_cutterLooksOverlayがセットされていません"));
}

void UInGameUI::SetCutterImage(UTexture2D* texture)
{
	if (_cutterLooksImage)
	{
		_cutterLooksImage->SetBrushFromTexture(texture);
	}
	else UE_LOG(LogTemp, Error, TEXT("_cutterLooksImageがセットされていません"));
}

void UInGameUI::SetScore(int score, float deltaTime)
{
	if (_countAnimatedScore < score)
	{
		_countAnimatedScore = FMath::Min(_countAnimatedScore + _countUpSpeed * deltaTime, score);
	}
	else if (_countAnimatedScore > score)
	{
		_countAnimatedScore = FMath::Max(_countAnimatedScore - _countUpSpeed * deltaTime, score);
	}
	
	_scoreText->SetText(CutterFormat::ConvertScoreText(_countAnimatedScore));
}

void UInGameUI::SetTime(float limitTime) const
{
	FText&& formattedTime = CutterFormat::ConvertTimeText(limitTime);
	_timeText->SetText(formattedTime);
}