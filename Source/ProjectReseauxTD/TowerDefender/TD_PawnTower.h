// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectReseauxTD/GameData/PlayerController/TD_Pawn.h"
#include "ProjectReseauxTD/GameData/SpawnableStruct.h"

#include "TD_PawnTower.generated.h"

UCLASS()
class PROJECTRESEAUXTD_API ATD_PawnTower : public ATD_Pawn
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this pawn's properties
	ATD_PawnTower();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSpawnableData CurrentTowerData;

	UFUNCTION(BlueprintCallable)
	void SelectTower(const FSpawnableData& data);

private:
	bool holdingTower = false;
};
