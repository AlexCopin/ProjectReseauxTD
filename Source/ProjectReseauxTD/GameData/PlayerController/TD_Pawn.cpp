// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_Pawn.h"

// Sets default values
ATD_Pawn::ATD_Pawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATD_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void ATD_Pawn::BeginPlay()
{
	Super::BeginPlay();
}
// Called to bind functionality to input
void ATD_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

