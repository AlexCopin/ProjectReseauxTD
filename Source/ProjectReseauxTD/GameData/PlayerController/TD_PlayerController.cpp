// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_PlayerController.h"
#include "ProjectReseauxTD/GameData/GameInstance/TD_NetworkSubsystem.h"

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
		GoldWidget = CreateWidget<UGoldWidget>(this, GoldWidgetClass);
		if(ensure(GoldWidget))
			GoldWidget->AddToViewport();
	}
}

void ATD_PlayerController::SpawnRightPawn(EPlayerType playerType)
{
	switch (playerType)
	{
	case EPlayerType::Attacker:
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Red, TEXT("Attacker spawn pawn"));
		//Spawn pawn attacker
		break;
	case EPlayerType::Defender:
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Red, TEXT("Defender spawn pawn"));
		//Spawn pawn Defender
		break;
	case EPlayerType::Spectator:
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Red, TEXT("Spectator spawn pawn"));
		//Spawn pawn Spectator
		break;
	}
}

void ATD_PlayerController::UpdateGold(int32 value)
{
	int32 diff = value - CurrentGold;
	CurrentGold = value;
	GoldWidget->UpdateGold(value, diff);
}
