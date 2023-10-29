#include "TD_PawnAttacker.h"
#include "ProjectReseauxTD/GameData/GameInstance/TD_NetworkSubsystem.h"	


ATD_PawnAttacker::ATD_PawnAttacker()
{
}

void ATD_PawnAttacker::BeginPlay()
{
	Super::BeginPlay();
}

void ATD_PawnAttacker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATD_PawnAttacker::SelectEnemy(const FSpawnableData& data)
{
	if (auto NetworkSS = GetWorld()->GetGameInstance()->GetSubsystem<UTD_NetworkSubsystem>())
	{
		FEnemySpawnClientPacket enemySpawnClientPacket;
		enemySpawnClientPacket.line = 0;
		enemySpawnClientPacket.enemyType = (EEnemyType)data.EnumValue;

    FString debugString = FString::Printf(TEXT("Try/Send spawn Enemy"));
    GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Red, *debugString);

		NetworkSS->SendEnemySpawnClientPacket(enemySpawnClientPacket);
	}
}
