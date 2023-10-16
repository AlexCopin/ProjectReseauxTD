// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "ProjectReseauxTD/GameData/SpawnableStruct.h"
#include "TD_Tower.generated.h"

UCLASS()
class PROJECTRESEAUXTD_API ATD_Tower : public AActor
{
	GENERATED_BODY()
	
public:	
	TObjectPtr<USceneComponent> Root;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UArrowComponent> TopPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FSpawnableData TowerData;

	// Sets default values for this actor's properties
	ATD_Tower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Shoot(AActor& Enemie);

};
