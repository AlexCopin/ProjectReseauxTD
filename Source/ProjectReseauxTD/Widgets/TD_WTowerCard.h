#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "Components/Button.h"
#include "ProjectReseauxTD/GameData/SpawnableStruct.h"

#include "TD_WTowerCard.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTowerCardSelected, const FSpawnableData&, data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyCardSelected, const FSpawnableData&, data);

UCLASS()
class UTD_WTowerCard : public UUserWidget
{
	GENERATED_BODY()
public:

	void Init(const FSpawnableData& data);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSpawnableData SpawnableData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URichTextBlock> Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URichTextBlock> Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta= (BindWidget))
	TObjectPtr<UButton> Button;

	UFUNCTION()
	void SelectThisCard();

	UPROPERTY(BlueprintAssignable)
	FOnTowerCardSelected OnTowerCardSelectedEvent;
	
	UPROPERTY(BlueprintAssignable)
	FOnEnemyCardSelected OnEnemyCardSelectedEvent;
};