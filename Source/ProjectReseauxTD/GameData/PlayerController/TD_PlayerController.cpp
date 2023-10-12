// Fill out your copyright notice in the Description page of Project Settings.


#include "TD_PlayerController.h"

ATD_PlayerController::ATD_PlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneComponent);
}
