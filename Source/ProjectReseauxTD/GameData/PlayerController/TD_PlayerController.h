// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TD_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRESEAUXTD_API ATD_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATD_PlayerController(const FObjectInitializer& ObjectInitializer);



	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<USceneComponent> SceneComponent;
};
