#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Service/X_Types.h"
#include "X_ButtonWithText.generated.h"


class UTextBlock;
class UButton;

UCLASS()
class X4STATIONBUILDING_API UX_ButtonWithText : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnButtonPressed();

public:

	void SetText(const FText InText) const;
	void SetText(const FName InText) const;

public:

	FTextDelegate OnButtonPressedEvent;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* Button;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* Text;
};
