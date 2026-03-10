#include "Application/CutterFormat.h"

FText CutterFormat::ConvertTimeText(float time)
{
	int integerTime = time;
	int floats = (time - integerTime) * 1000;
	int seconds = integerTime % 60;
	int minutes = (integerTime % 3600)/60;
	
	FText formattedTime = FText::Format(
		FText::FromString(TEXT("{0}:{1}.{2}")),
			FText::FromString(FString::Printf(TEXT("%02d"),minutes)),
			FText::FromString(FString::Printf(TEXT("%02d"),seconds)),
			FText::FromString(FString::Printf(TEXT("%03d"),floats))
			);
	
	return formattedTime;
}

FText CutterFormat::ConvertScoreText(int score)
{
	FNumberFormattingOptions textFormatOptions = {};
	textFormatOptions.SetMaximumIntegralDigits(6);
	textFormatOptions.SetMinimumIntegralDigits(6);
	textFormatOptions.SetUseGrouping(false);
	return FText::AsNumber(score, &textFormatOptions);
}

FString CutterFormat::ConvertOpenLevelOption(FString key, FString value)
{
	return "?" + key + "=" + value;
}