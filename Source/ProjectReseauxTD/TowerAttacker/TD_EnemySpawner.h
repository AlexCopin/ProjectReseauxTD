// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TargetPoint.h"
#include "TD_Enemy.h"
#include "ProjectReseauxTD/GameData/Protocol.h"
#include "TD_EnemySpawner.generated.h"

UCLASS()
class PROJECTRESEAUXTD_API ATD_EnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ATD_EnemySpawner(const FObjectInitializer& OI);

	UFUNCTION()
	void SpawnEnemy(FEnemySpawnServerPacket enemySpawnServerPacket);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySpawner")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySpawner")
	TObjectPtr<ATargetPoint> TargetPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySpawner")
	TSubclassOf<ATD_Enemy> EnemyActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
