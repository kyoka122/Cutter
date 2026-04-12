#pragma once

#include "CoreMinimal.h"

/**
 * 表示フォーマットの定義クラス
 * （テキスト表示フォーマット、スコア表示フォーマット等）
 */
class CutterFormat
{
public:
	/* 残り時間表示フォーマット */
	static FText ConvertTimeText(float time);
	
	/* スコア表示フォーマット */
	static FText ConvertScoreText(int score);
	
	/* レベル遷移時パラメータフォーマット */
	static FString ConvertOpenLevelOption(const FString& key, const FString& value);
};
