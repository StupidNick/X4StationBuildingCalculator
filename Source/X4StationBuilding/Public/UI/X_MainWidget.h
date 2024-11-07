#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Service/X_Types.h"
#include "X_MainWidget.generated.h"

class UX_CheckBoxWithText;
class UX_NameWithAmountWD;
class UVerticalBox;
class UScrollBox;
class UX_DropDownButton;
class UEditableText;
class UTextBlock;
class UX_DropDownMenu;
class UButton;
struct FObjectInfo;

UCLASS()
class X4STATIONBUILDING_API UX_MainWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	void ClearSelectedStationsList();
	void SetResult(FResult& InResult);

	void PrintError(const FText& InText) const;
	void PrintError(const FString& InText) const;

private:
	UFUNCTION()
	void OnAddButtonClicked();
	UFUNCTION()
	void OnAutofillButtonClicked();
	UFUNCTION()
	void OnClearSelectedListButtonClicked();

	void SetProductsInfo(FResult& InResult);
	void SetWorkforceInfo(const FResult& InResult);
	void SetProductionCostInfo(const FResult& InResult);
	void SetStationCostInfo(const FResult& InResult);

	void CreateResourcesPerHourButton(const TArray<FProductCostInfo>& InInfo, const int32 InTotalCost);

	void ClearResults();
	UX_DropDownMenu* AddStationLine();

public:

	FTextInt32Delegate AddNewStationEvent;
	FChangeStationsCountDelegate ChangeStationsCountEvent;
	FTextInt32Delegate RemoveStationEvent;

	FFillStationsDelegate CalculateStationsEvent;
	
	FSimpleDelegate OnClearSelectedListButtonClickedEvent;

	FBoolDelegate OnProvideBasicResourceValueChanged;
	FBoolDelegate OnProvideAllResourceValueChanged;

	UPROPERTY(EditDefaultsOnly, Category = "DropDown")
	TSubclassOf<UX_DropDownButton> DropDownButtonClass;

	UPROPERTY(EditDefaultsOnly, Category = "DropDown")
	FMargin DropDownButtonsPadding;

	UPROPERTY(EditDefaultsOnly, Category = "DropDown")
	TSubclassOf<UX_DropDownMenu> SelectStationClass;

	UPROPERTY(EditDefaultsOnly, Category = "Common")
	TSubclassOf<UX_NameWithAmountWD> NameWithAmountClass;

	UPROPERTY(EditDefaultsOnly, Category = "Common")
	FText ProfitName;

	UPROPERTY(EditDefaultsOnly, Category = "Common")
	FText BasicResourcesCheckBoxName;

	UPROPERTY(EditDefaultsOnly, Category = "Common")
	FText AllResourcesCheckBoxName;

	UPROPERTY(EditDefaultsOnly, Category = "Common")
	FText TotalCostStationName;
	
// TextBlocks begin
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ErrorTextBlock;
// TextBlocks end

// Buttons begin
	UPROPERTY(meta = (BindWidget))
	UButton* AddStationsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* AutofillButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ClearSelectedListButton;

	UPROPERTY(meta = (BindWidget))
	UX_CheckBoxWithText* ProvideBasicResourcesCB;
	
	UPROPERTY(meta = (BindWidget))
	UX_CheckBoxWithText* ProvideAllResourcesCB;
// Buttons end

// Boxes begin
	UPROPERTY(meta = (BindWidget))
	UScrollBox* MainSB;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* OutputProductsVB;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* OutputWorkforceVB;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* OutputProductsCostVB;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* OutputStationCostVB;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* SelectedStationsVB;
// Boxes end

private:

	UPROPERTY()
	TArray<UX_DropDownMenu*> SelectedStations;

	UPROPERTY()
	TArray<UX_DropDownButton*> DropDownButtons;

	UPROPERTY()
	TArray<UX_NameWithAmountWD*> LinesWithAmount;
};
