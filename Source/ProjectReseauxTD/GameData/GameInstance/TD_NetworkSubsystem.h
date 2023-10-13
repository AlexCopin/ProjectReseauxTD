// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define NOMINMAX
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include <enet6/enet.h>
#include <ProjectReseauxTD/GameData/Protocol.h>
#include "TD_NetworkSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawnReceived, FEnemySpawnServerPacket, packet);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTowerSpawnReceived, FTowerSpawnServerPacket, packet);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyPositionReceived, FEnemyPositionServerPacket, packet);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerInit, EPlayerType, playerType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoldChange, int32, value);

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
	void SendEnemySpawnClientPacket(const FEnemySpawnClientPacket& packet);
	
	UFUNCTION(BlueprintCallable)
	void SendEnemyPathClientPacket(const FEnemyPathClientPacket& packet);

	UFUNCTION(BlueprintCallable)
	void SendSpawnTowerClientPacket(const FTowerSpawnClientPacket& packet);

	UFUNCTION(BlueprintCallable)
	void SendCastlePositionPacket(const FCastlePositionClientPacket& packet);

	UPROPERTY(BlueprintAssignable)
	FOnEnemySpawnReceived OnEnemySpawnEvent;

	UPROPERTY(BlueprintAssignable)
	FOnTowerSpawnReceived OnTowerSpawnEvent;
	
	UPROPERTY(BlueprintAssignable)
	FOnEnemyPositionReceived OnEnemyPositionEvent;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerInit OnPlayerInitEvent;

	UPROPERTY(BlueprintAssignable)
	FOnGoldChange OnGoldChangeEvent;

private:
	void handle_message(const std::vector<std::uint8_t>& message);
	void send_packet(ENetPacket* packet);
	ENetHost* Host = nullptr;
	ENetPeer* ServerPeer = nullptr;
};
