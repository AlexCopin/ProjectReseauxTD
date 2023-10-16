#include "TD_WPlayerWidget.h"

void UTD_WPlayerWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTD_WPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTD_WPlayerWidget::AddSpawnableData(const FSpawnableData& spawnableData)
{
	auto card = CreateWidget<UTD_WTowerCard>(this, TowerCardClass);
	CardsBox->AddChild(card);
	card->Init(spawnableData);
}
