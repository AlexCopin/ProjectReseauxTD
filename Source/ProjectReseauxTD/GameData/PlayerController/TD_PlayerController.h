// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectReseauxTD/GameData/SpawnableStruct.h"

#include "TD_PlayerController.generated.h"

class UTD_GoldWidget;
class ATD_PawnAttacker;
class ATD_PawnTower;
class UTD_WPlayerWidget;
class ATD_Tower;
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
	void ReceiveData(const FSpawnableData& spawnableData, bool inWidget);

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

	UFUNCTION()
	void SpawnTower(FTowerSpawnServerPacket ReceivedPacket);
	//Array ou map � terme, pour l'instant m�me mod�le donc osef
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ATD_Tower> TowerBaseClass;

	UPROPERTY()
	TMap<uint8, FSpawnableData> Data;
	UPROPERTY()
	TMap<int32, TObjectPtr<ATD_Tower>> Towers;

  EPlayerType PlayerType;
};
