#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "X_DropDownButtonWD.generated.h"


class UVerticalBox;
struct FResult;
class UX_ObjectsListWD;
class UTextBlock;
class UButton;

UCLASS()
class X4STATIONBUILDING_API UX_DropDownButton : public UUserWidget
{
	GENERATED_BODY()

public:

	void InitializeWidget(FText InName, FResult& InResult);

private:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OpenMenu();
	void SetTextColor(const FLinearColor& InColor) const;

public:

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* DetailsVerticalBox;

	UPROPERTY(meta = (BindWidget))
	UButton* OpenButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmountTextBlock;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UX_ObjectsListWD> ListClass;

private:

	UPROPERTY()
	UX_ObjectsListWD* List;

	bool bIsOpen = false;
};
