// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_PawnTower.h"

// Sets default values
ATD_PawnTower::ATD_PawnTower()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATD_PawnTower::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ATD_PawnTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(holdingTower)
	{
		FVector position;
		FVector direction;
		FHitResult hit;
		GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(CursorPosition, direction);
		CursorPositionOnFloor *= FVector::UpVector;
		FString debugString = FString::Printf(TEXT("Vector mouse pos =  %f - %f - %f"), CursorPositionOnFloor.X, CursorPositionOnFloor.Y, CursorPositionOnFloor.Z);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.5f, FColor::Red, *debugString);
		//GetWorld()->LineTraceSingleByChannel(hit, position, FVector::DownVector * 100, ECollisionChannel::ECC_EngineTraceChannel3);
		DrawDebugCircle(GetWorld(), CursorPositionOnFloor, CurrentTowerData.Radius, 50, FColor::Red, false, 0.1f);
		DrawDebugCircle(GetWorld(), CursorPositionOnFloor, CurrentTowerData.Radius + CurrentTowerData.Range, 50, FColor::Orange, false, 0.1f);
	}
}

// Called to bind functionality to input
void ATD_PawnTower::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATD_PawnTower::SelectTower(const FSpawnableData& data)
{
	CurrentTowerData = data;
	holdingTower = true;
}

