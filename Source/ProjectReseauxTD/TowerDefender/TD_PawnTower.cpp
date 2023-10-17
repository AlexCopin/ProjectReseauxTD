// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_PawnTower.h"
#include "ProjectReseauxTD/GameData/GameInstance/TD_NetworkSubsystem.h"

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
		CursorPositionOnFloor = GetActorLocation();
		CursorPositionOnFloor.RotateAngleAxis(90, FVector(0, 1, 0));
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


void ATD_PawnTower::TrySpawnTower()
{
	if (auto NetworkSS = GetWorld()->GetGameInstance()->GetSubsystem<UTD_NetworkSubsystem>())
	{
		FTowerSpawnClientPacket packet;
		packet.towerType = (ETowerType)CurrentTowerData.EnumValue;
		packet.position = GetActorLocation();
		NetworkSS->SendSpawnTowerClientPacket(packet);
	}
}
