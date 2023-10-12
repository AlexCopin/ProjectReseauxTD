// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_EnemySpawner.h"

// Sets default values
ATD_EnemySpawner::ATD_EnemySpawner(const FObjectInitializer& OI) : Super(OI)
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

// Called when the game starts or when spawned
void ATD_EnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	auto spawnedEnemy = GetWorld()->SpawnActor<ATD_Enemy>(EnemyActor, GetActorTransform());
	spawnedEnemy->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	spawnedEnemy->SetActorRelativeLocation(FVector(0));
	spawnedEnemy->Initialize(TargetPoint);
	spawnedEnemy->EnemySpawned();
}

// Called every frame
void ATD_EnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

