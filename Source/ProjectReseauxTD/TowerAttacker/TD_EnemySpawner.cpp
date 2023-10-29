// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_EnemySpawner.h"
#include "ProjectReseauxTD/GameData/GameInstance/TD_NetworkSubsystem.h"	

// Sets default values
ATD_EnemySpawner::ATD_EnemySpawner(const FObjectInitializer& OI) : Super(OI)
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

void ATD_EnemySpawner::SpawnEnemy(FEnemySpawnServerPacket enemySpawnServerPacket)
{
	auto spawnedEnemy = GetWorld()->SpawnActor<ATD_Enemy>(EnemyActor, GetActorTransform());
  if(spawnedEnemy)
  {
	  //spawnedEnemy->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	  //spawnedEnemy->MoveToLocation(FVector(0));
    spawnedEnemy->SetActorLocation(GetActorLocation());
	  spawnedEnemy->Initialize(TargetPoint, enemySpawnServerPacket);
	  spawnedEnemy->EnemySpawned();
  }
}

// Called when the game starts or when spawned
void ATD_EnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto NetworkSS = GetWorld()->GetGameInstance()->GetSubsystem<UTD_NetworkSubsystem>())
	{
		NetworkSS->OnEnemySpawnEvent.AddDynamic(this, &ATD_EnemySpawner::SpawnEnemy);

		FEnemySpawnClientPacket enemySpawnClientPacket;
		enemySpawnClientPacket.line = 1;
		enemySpawnClientPacket.enemyType = EEnemyType::Tank;

		NetworkSS->SendEnemySpawnClientPacket(enemySpawnClientPacket);
	}
}

// Called every frame
void ATD_EnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

