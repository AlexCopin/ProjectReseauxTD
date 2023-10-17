// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/TargetPoint.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "TD_Enemy.generated.h"

UCLASS()
class PROJECTRESEAUXTD_API ATD_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATD_Enemy(const FObjectInitializer& OI);
	void Initialize(TObjectPtr<ATargetPoint> targetPoint);

	UFUNCTION(BlueprintImplementableEvent)
	void EnemySpawned();
	
	UFUNCTION(BlueprintImplementableEvent)
	void MoveTo(FEnemyPositionServerPacket enemyPositionServerPacket);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	TObjectPtr<ATargetPoint> TargetPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	TObjectPtr<UNavigationPath> TPath;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	FVector NextPoint;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
