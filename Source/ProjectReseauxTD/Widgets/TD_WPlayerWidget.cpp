#include "TD_WPlayerWidget.h"
#include "ProjectReseauxTD/TowerAttacker/TD_PawnAttacker.h"
#include "ProjectReseauxTD/TowerDefender/TD_PawnTower.h"

void UTD_WPlayerWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTD_WPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTD_WPlayerWidget::AddSpawnableData(const FSpawnableData& spawnableData, ATD_Pawn* pawn)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Red, TEXT("AddSpawnableData - PlayerWidget"));
	auto card = CreateWidget<UTD_WTowerCard>(this, TowerCardClass);
	CardsBox->AddChild(card);
	card->Init(spawnableData);
	if(auto castedPawn = Cast<ATD_PawnAttacker>(pawn))
	{}
	else if (ATD_PawnTower* castedPawnDefender = reinterpret_cast<ATD_PawnTower*>(pawn))
		card->OnTowerCardSelectedEvent.AddDynamic(castedPawnDefender, &ATD_PawnTower::SelectTower);
	
}
