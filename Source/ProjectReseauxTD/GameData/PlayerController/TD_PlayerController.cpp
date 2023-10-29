// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_PlayerController.h"
#include "ProjectReseauxTD/GameData/GameInstance/TD_NetworkSubsystem.h"
#include "ProjectReseauxTD/TowerAttacker/TD_PawnAttacker.h"
#include "ProjectReseauxTD/TowerDefender/TD_PawnTower.h"
#include "ProjectReseauxTD/Gold/TD_GoldWidget.h"
#include "ProjectReseauxTD/Widgets/TD_WPlayerWidget.h"
#include "ProjectReseauxTD/TowerDefender/TD_Tower.h"
#include "ProjectReseauxTD/GameData/Protocol.h"

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
		NetworkSS->OnTowerSpawnEvent.AddDynamic(this, &ATD_PlayerController::SpawnTower);
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
  PlayerType = playerType;
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
			PlayerWidget = CreateWidget<UTD_WPlayerWidget>(this, pawn->PawnWidgetClass);
			PlayerWidget->AddToViewport();
			if (!SpawnableDatas.IsEmpty())
			{
				for (const auto& data : SpawnableDatas)
				{
					ReceiveData(data, true);
				}
			}
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
			PlayerWidget = CreateWidget<UTD_WPlayerWidget>(this, pawn->PawnWidgetClass);
			PlayerWidget->AddToViewport();
			if (!SpawnableDatas.IsEmpty())
			{
				for (const auto& data : SpawnableDatas)
				{
					ReceiveData(data, true);
				}
			}
			break;
		}
		case EPlayerType::Spectator:
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.0f, FColor::Red, TEXT("Spectator spawn pawn"));
			//Spawn pawn Spectator
			break;
		}
	}
}

void ATD_PlayerController::ReceiveData(const FSpawnableData& spawnableData, bool inWidget)
{
	if(PlayerWidget && inWidget)
		PlayerWidget->AddSpawnableData(spawnableData, GetPawn<ATD_Pawn>());
	else
		SpawnableDatas.Add(spawnableData);

	Data.Emplace(spawnableData.EnumValue, spawnableData);
}

void ATD_PlayerController::UpdateGold(int32 value)
{
	int32 diff = value - CurrentGold;
	CurrentGold = value;
	GoldWidget->UpdateGold(value, diff);
}

void ATD_PlayerController::SpawnTower(FTowerSpawnServerPacket ReceivedPacket)
{
	auto NewTower = GetWorld()->SpawnActor<ATD_Tower>(TowerBaseClass, ReceivedPacket.position, FRotator::ZeroRotator);
	Towers.Emplace(ReceivedPacket.index, NewTower);
  NewTower->ChangeMesh(ReceivedPacket.towerType);
  if (PlayerType == EPlayerType::Defender)
  {
	  FSpawnableData data = *Data.Find(ReceivedPacket.towerType);
	  NewTower->Init(data);
  }
}
