#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "ProjectReseauxTD/GameData/SpawnableStruct.h"
#include "ProjectReseauxTD/Widgets/TD_WTowerCard.h"


#include "TD_WPlayerWidget.generated.h"


UCLASS()
class UTD_WPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void NativePreConstruct() override;
	void NativeConstruct() override;

	UFUNCTION()
	void AddSpawnableData(const FSpawnableData& spawnableData);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> CardsBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UTD_WTowerCard> TowerCardClass;
};