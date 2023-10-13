#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"


#include "TD_GoldWidget.generated.h"


UCLASS()
class UTD_GoldWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URichTextBlock> ValueText;

	UFUNCTION()
	void UpdateGold(int32 Value, int32 diffValue);
};