// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectReseauxTD/Gold/GoldWidget.h"

#include "TD_PlayerController.generated.h"

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

	//Gold
	UPROPERTY()
	int32 CurrentGold;

	UFUNCTION()
	void UpdateGold(int32 value);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGoldWidget> GoldWidgetClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UGoldWidget> GoldWidget;

private:

};
