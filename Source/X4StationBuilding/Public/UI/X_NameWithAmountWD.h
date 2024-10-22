#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "X_NameWithAmountWD.generated.h"


class UTextBlock;

UCLASS()
class X4STATIONBUILDING_API UX_NameWithAmountWD : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetInfo(const FText& InNameObject, int32 InAmountObjects) const;

public:

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* NameTextBlock;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* AmountTextBlock;
};
