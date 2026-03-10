#pragma once

#include "CoreMinimal.h"

/**
 * 表示フォーマットの定義クラス
 * （テキスト表示フォーマット、スコア表示フォーマット等）
 */
class CutterFormat
{
public:
	static FText ConvertTimeText(float time);
	static FText ConvertScoreText(int score);
	static FString ConvertOpenLevelOption(FString key, FString value);
};
