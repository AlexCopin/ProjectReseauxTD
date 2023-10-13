#include "TD_GoldWidget.h"

void UTD_GoldWidget::UpdateGold(int32 Value, int32 diffValue)
{
	ValueText->SetText(FText::FromString(FString::FromInt(Value)));
}
