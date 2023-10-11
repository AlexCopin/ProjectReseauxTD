// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include <enet6/enet.h>
#include <ProjectReseauxTD/GameData/Protocol.h>
#include "TD_NetworkSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawnReceived, FEnemySpawnServerPacket, packet);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTowerSpawnReceived, FTowerSpawnServerPacket, packet);

UCLASS()
class PROJECTRESEAUXTD_API UTD_NetworkSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	ETickableTickType GetTickableTickType() const override;
	bool IsAllowedToTick() const override final;
	void Tick(float DeltaTime) override;
	TStatId GetStatId() const override;

	UFUNCTION(BlueprintCallable)
	bool Connect(FString AddressString);

	UFUNCTION(BlueprintCallable)
	void Disconnect();

	void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable)
	void SendEnemySpawnClientPacket(FEnemySpawnClientPacket packet);

	UFUNCTION(BlueprintCallable)
	void SendSpawnTowerClientPacket(FTowerSpawnClientPacket packet);

	UPROPERTY(BlueprintAssignable)
	FOnEnemySpawnReceived OnEnemySpawnEvent;
	UPROPERTY(BlueprintAssignable)
	FOnTowerSpawnReceived OnTowerSpawnEvent;

private:
	void handle_message(const std::vector<std::uint8_t>& message);
	void send_packet(ENetPacket* packet);
	ENetHost* Host = nullptr;
	ENetPeer* ServerPeer = nullptr;
};
