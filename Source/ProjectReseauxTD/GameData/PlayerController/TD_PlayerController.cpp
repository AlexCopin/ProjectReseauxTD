// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_PlayerController.h"
#include "ProjectReseauxTD/GameData/GameInstance/TD_NetworkSubsystem.h"
#include "ProjectReseauxTD/TowerAttacker/TD_PawnAttacker.h"
#include "ProjectReseauxTD/TowerDefender/TD_PawnTower.h"
#include "ProjectReseauxTD/Gold/TD_GoldWidget.h"
#include "ProjectReseauxTD/Widgets/TD_WPlayerWidget.h"

ATD_PlayerController::ATD_PlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneComponent);
}

void ATD_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	if(auto NetworkSS = GetWorld()->GetGameInstance()->GetSubsystem<UTD_NetworkSubsystem>())
	{
		NetworkSS->OnPlayerInitEvent.AddDynamic(this, &ATD_PlayerController::SpawnRightPawn);
		NetworkSS->OnGoldChangeEvent.AddDynamic(this, &ATD_PlayerController::UpdateGold);
	}
	if(ensure(GoldWidgetClass))
	{
		GoldWidget = CreateWidget<UTD_GoldWidget>(this, GoldWidgetClass);
		if(ensure(GoldWidget))
			GoldWidget->AddToViewport();
	}
}

void ATD_PlayerController::SpawnRightPawn(EPlayerType playerType)
{
	switch (playerType)
	{
	case EPlayerType::Attacker:
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Red, TEXT("Attacker spawn pawn"));
		ATD_PawnAttacker* pawn = GetWorld()->SpawnActor<ATD_PawnAttacker>(PawnAttackerClass);
		pawn->SetOwner(this);
		UnPossess();
		SetPawn(pawn);
		Possess(pawn);
		break;
	}
	case EPlayerType::Defender:
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Red, TEXT("Defender spawn pawn"));
		ATD_PawnTower* pawn = GetWorld()->SpawnActor<ATD_PawnTower>(PawnTowerClass);
		pawn->SetOwner(this);
		UnPossess();
		SetPawn(pawn);
		Possess(pawn);
		//Spawn pawn Defender
		break;
	}
	case EPlayerType::Spectator:
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Red, TEXT("Spectator spawn pawn"));
		//Spawn pawn Spectator
		break;
	}
	if(auto pawn = Cast<ATD_Pawn>(GetPawn()))
	{
		PlayerWidget = CreateWidget<UTD_WPlayerWidget>(this, pawn->PawnWidgetClass);
		PlayerWidget->AddToViewport();
	}
}

void ATD_PlayerController::ReceiveTowerData(const FSpawnableData& spawnableData)
{
	if(PlayerWidget)
		PlayerWidget->AddSpawnableData(spawnableData);
}

void ATD_PlayerController::UpdateGold(int32 value)
{
	int32 diff = value - CurrentGold;
	CurrentGold = value;
	GoldWidget->UpdateGold(value, diff);
}
