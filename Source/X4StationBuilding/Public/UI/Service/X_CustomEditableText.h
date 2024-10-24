#pragma once

#include "CoreMinimal.h"
#include "Components/EditableText.h"
#include "X_Types.h"
#include "X_CustomEditableText.generated.h"



UCLASS()
class X4STATIONBUILDING_API UX_CustomEditableText : public UEditableText
{
	GENERATED_BODY()

public:

	void InitializeWidget();

	UFUNCTION()
	void OnTextChangedCallback(const FText& InText);

public:
	
	FInt32Delegate OnEditedTextChangedEvent;

	UPROPERTY(EditAnywhere, Category = "Custom")
	int32 MaxSymbolCount = 4;
};
