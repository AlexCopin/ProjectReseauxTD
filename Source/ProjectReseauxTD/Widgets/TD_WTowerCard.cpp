#include "TD_WTowerCard.h"

void UTD_WTowerCard::Init(const FSpawnableData& data)
{
	SpawnableData = data;
	Name->SetText(FText::FromString(data.Name));
	Cost->SetText(FText::FromString(FString::FromInt(data.Cost)));
}
