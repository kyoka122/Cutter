#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGame/Interface/CutterLooksView.h"
#include "InGame/Interface/OverViewMiniMap.h"
#include "InGameUI.generated.h"

class UMinimapUI;
class UOverlay;
class AInGameState;
class UImage;
class UTextBlock;

/**
 * インゲームで表示するUI全てを管理するクラス
 */
UCLASS()
class UInGameUI : public UUserWidget, public IOverViewMiniMap, public ICutterLooksView
{
	GENERATED_BODY()
	
public:
	/*表示コンテンツの更新*/
	void UpdateUI(const AInGameState* gameState, float deltaTime);
	
	/*ブロワーのアイコン表示数更新*/
	void SetBlowerCount(int blowerCount);

	/*Miniマップの表示、非表示*/
	virtual void SetVisibilityMiniMap(bool value) override;
	
	/*描画する線情報を登録する*/
	virtual void UpdateDrawLines(const TArray<FVector2D>& points, FVector cameraPos, float cameraOrthoWidth) override;

	/*カッターイメージの表示表示非表示を切り替える*/
	virtual void SetVisibilityCutterLooksView(bool value) override;
	
	/*Cutter画像をセットする*/
	virtual void SetCutterImage(UTexture2D* texture) override;
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> _scoreText = {};
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> _timeText = {};
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> _blowerCountText = {};
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UOverlay> _blowerImagesOverlay = {};
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> _cutterLooksImage = {};
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UOverlay> _cutterLooksOverlay = {};
	
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UOverlay> _miniMapOverlay = {};
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMinimapUI> _miniMap = {};
	
	UPROPERTY(EditAnywhere, Category = "UI設定")
	float _countUpSpeed = 1;
	
	UPROPERTY(EditAnywhere, Category = "UI設定")
	TSubclassOf<UUserWidget> _blowerImage = {};

private:
	void SetScore(int score, float deltaTime);
	void SetTime(float limitTime) const;
	
private:
	float _countAnimatedScore = 0;//MEMO: スコアのカウントアニメーション実装のため、現在表示している数値をキャッシュしておく
	float _time = 0;
	int32 _currentSpawnedBlowerCount = 0;
	
	TArray<TObjectPtr<UUserWidget>> _blowerImages = {};
};
