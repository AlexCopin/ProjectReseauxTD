#pragma once

#include "SpawnableStruct.generated.h"

USTRUCT(BlueprintType)
struct PROJECTRESEAUXTD_API FSpawnableData
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Range = 500;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Radius = 500;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Cost = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<AActor> Actor;
};