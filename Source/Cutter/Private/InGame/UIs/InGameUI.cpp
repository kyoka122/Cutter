#include "InGame/UIs/InGameUI.h"

#include <string>

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
			UE_LOG(LogTemp, Error, TEXT("scrollBoxSlotの生成に失敗しました levelName: %s"), *GetName());
			return;
		}
	}
	_currentSpawnedBlowerCount = blowerCount;
}

void UInGameUI::SetVisibilityMiniMap(bool value)
{
	if (!_miniMapOverlay)
	{
		UE_LOG(LogTemp, Error, TEXT("_miniMapが見つかりませんでした。"));
		return;
	}
	ESlateVisibility visible = value ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	_miniMapOverlay->SetVisibility(visible);
}

void UInGameUI::UpdateDrawLines(const TArray<FVector2D>& points, FVector cameraPos, float cameraOrthoWidth)
{
	_points.Reset();//メモリを保持したまま要素数0に
	_points.Reserve(points.Num());
	for (const auto& point : points)
	{
		FVector2D normalizedPoint = (point - FVector2D(cameraPos)) / cameraOrthoWidth;//clippingに変換
		FVector2D widgetPoint(normalizedPoint.Y + 0.5f, -normalizedPoint.X + 0.5f);//Widgetの座標系に変換
		_points.Add(widgetPoint);
	}
}

int32 UInGameUI::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 layerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	if (_points.Num() <= 0 || !_miniMapOverlay->IsVisible())
	{
		return layerId;
	}
	if (_miniMap)
	{
		FGeometry MiniMapGeometry = _miniMap->GetCachedGeometry();
		FGeometry RelativeMiniMapGeometry = AllottedGeometry.MakeChild(
			MiniMapGeometry.GetLocalSize(),
			FSlateLayoutTransform(AllottedGeometry.AbsoluteToLocal(MiniMapGeometry.GetAbsolutePosition()))
			);
		
		DrawCutterPath(RelativeMiniMapGeometry, OutDrawElements, layerId + 1);
	}
	else UE_LOG(LogTemp, Error, TEXT("_miniMapが見つかりませんでした。"));
	
	return layerId;
}

void UInGameUI::DrawCutterPath(const FGeometry& geometry, FSlateWindowElementList& outDrawElements, int32 layerId) const
{
	FVector2D localSize = geometry.GetLocalSize();
	TArray<FVector2D> points;
	points.Reserve(_points.Num());
	for (auto point : _points)
	{
		//UE_LOG(LogTemp, Log, TEXT("point * localSize: %s"), *(point * localSize).ToString());
		points.Add(point * localSize);//X,Yが逆になる
	}
	
	FSlateDrawElement::MakeLines(outDrawElements,
		layerId,
		geometry.ToPaintGeometry(),
		points,
		ESlateDrawEffect::None,
		FLinearColor::Red,
		true,
		_miniMapLineThickness);
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
