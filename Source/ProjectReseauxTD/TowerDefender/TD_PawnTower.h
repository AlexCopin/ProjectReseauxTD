// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TD_PawnTower.generated.h"

UCLASS()
class PROJECTRESEAUXTD_API ATD_PawnTower : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATD_PawnTower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
