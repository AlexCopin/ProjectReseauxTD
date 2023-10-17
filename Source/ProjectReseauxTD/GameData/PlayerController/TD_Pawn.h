// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ProjectReseauxTD/GameData/SpawnableStruct.h"

#include "TD_Pawn.generated.h"

class UTD_WPlayerWidget;

UCLASS()
class PROJECTRESEAUXTD_API ATD_Pawn : public APawn
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this pawn's properties
	ATD_Pawn();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CursorPositionOnFloor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CursorPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CursorPositionDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UTD_WPlayerWidget> PawnWidgetClass;
};
