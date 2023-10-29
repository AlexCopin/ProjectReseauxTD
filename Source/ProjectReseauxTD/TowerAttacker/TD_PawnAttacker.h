// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectReseauxTD/GameData/PlayerController/TD_Pawn.h"

#include "TD_PawnAttacker.generated.h"

class UTD_WPlayerWidget;

UCLASS()
class PROJECTRESEAUXTD_API ATD_PawnAttacker : public ATD_Pawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATD_PawnAttacker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SelectEnemy(const FSpawnableData& data);
};
