// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_Enemy.h"
#include "ProjectReseauxTD/GameData/GameInstance/TD_NetworkSubsystem.h"
#include <ProjectReseauxTD/GameData/Protocol.h>

// Sets default values
ATD_Enemy::ATD_Enemy(const FObjectInitializer& OI) : Super(OI)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATD_Enemy::Initialize(TObjectPtr<ATargetPoint> targetPoint, const FEnemySpawnServerPacket& infos)
{
	TargetPoint = targetPoint;
  index = infos.index;
}

// Called when the game starts or when spawned
void ATD_Enemy::BeginPlay()
{
	Super::BeginPlay();

	if (auto NetworkSS = GetWorld()->GetGameInstance()->GetSubsystem<UTD_NetworkSubsystem>())
	{
		NetworkSS->OnEnemyPositionEvent.AddDynamic(this, &ATD_Enemy::MoveEnemyTo);
	}
}

// Called every frame
void ATD_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
