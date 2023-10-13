#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"


#include "GoldWidget.generated.h"


UCLASS()
class UGoldWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URichTextBlock> ValueText;

	UFUNCTION()
	void UpdateGold(int32 Value, int32 diffValue);
};