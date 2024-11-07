#pragma once

#include "CoreMinimal.h"
#include "X_Types.h"
#include "Blueprint/UserWidget.h"
#include "X_CheckBoxWithText.generated.h"


class UTextBlock;
class UCheckBox;

UCLASS()
class X4STATIONBUILDING_API UX_CheckBoxWithText : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	void OnValueChanged(bool InValue);

	void SetText(const FText& InText) const;
	
public:
	FBoolDelegate OnValueChangedEvent;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* CheckBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text;
};
