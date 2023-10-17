// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "TD_PlayerController.generated.h"

class UTD_GoldWidget;
class ATD_PawnAttacker;
class ATD_PawnTower;
class UTD_WPlayerWidget;
/**
 * 
 */
UCLASS()
class PROJECTRESEAUXTD_API ATD_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATD_PlayerController(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;



	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<USceneComponent> SceneComponent;
	UFUNCTION()
	void SpawnRightPawn(EPlayerType playerType);

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<> AttackerPawnClass;*/

	//Pawns

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PC | Pawns")
	TSubclassOf<ATD_PawnAttacker> PawnAttackerClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PC | Pawns")
	TSubclassOf<ATD_PawnTower> PawnTowerClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FSpawnableData> SpawnableDatas;
	UFUNCTION()
	void ReceiveTowerData(const FSpawnableData& spawnableData);

	//Gold
	UPROPERTY()
	int32 CurrentGold;
	UFUNCTION()
	void UpdateGold(int32 value);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PC | Gold")
	TSubclassOf<UTD_GoldWidget> GoldWidgetClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTD_GoldWidget> GoldWidget;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTD_WPlayerWidget> PlayerWidget;

};
