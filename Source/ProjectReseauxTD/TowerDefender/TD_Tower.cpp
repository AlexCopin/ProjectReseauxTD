// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_Tower.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATD_Tower::ATD_Tower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	TopPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("TopPoint"));
	TopPoint->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ATD_Tower::BeginPlay()
{
	Super::BeginPlay();

	//DrawDebugSphere(GetWorld(), Root->GetComponentLocation(), Range, 20, FColor(255, 50, 255, 255), true);
}

// Called every frame
void ATD_Tower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATD_Tower::Shoot(AActor& Enemie)
{

	
}

