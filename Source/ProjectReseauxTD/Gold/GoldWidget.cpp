#include "GoldWidget.h"

void UGoldWidget::UpdateGold(int32 Value, int32 diffValue)
{
	ValueText->SetText(FText::FromString(FString::FromInt(Value)));
}
