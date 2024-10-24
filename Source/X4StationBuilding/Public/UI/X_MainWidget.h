#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Service/X_Types.h"
#include "X_MainWidget.generated.h"

struct FObjectInfo;
class UVerticalBox;
class UScrollBox;
class UX_DropDownButton;
class UEditableText;
class UTextBlock;
class UX_DropDownMenu;
class UButton;

UCLASS()
class X4STATIONBUILDING_API UX_MainWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	void SetStationsAndCount(TArray<FObjectInfo>& InStations);
	void ClearSelectedStationsList();
	void SetResult(FResult& InResult);

	void PrintError(const FText& InText) const;
	void PrintError(const FString& InText) const;

private:
	UFUNCTION()
	void OnAddButtonClicked();
	UFUNCTION()
	void OnCalculateButtonClicked();
	UFUNCTION()
	void OnClearSelectedListButtonClicked();

	void ClearResults();

	FString GetStringFromNamesAndNumbers(TArray<FObjectInfo>& InStations);

public:

	FTextInt32Delegate AddNewStationEvent;
	FChangeStationsCountDelegate ChangeStationsCountEvent;
	FTextInt32Delegate RemoveStationEvent;
	
	FSimpleDelegate OnCalculateButtonClickedEvent;
	FSimpleDelegate OnClearSelectedListButtonClickedEvent;

	UPROPERTY(EditDefaultsOnly, Category = "DropDown")
	TSubclassOf<UX_DropDownButton> DropDownButtonClass;

	UPROPERTY(EditDefaultsOnly, Category = "DropDown")
	FMargin DropDownButtonsPadding;

	UPROPERTY(EditDefaultsOnly, Category = "DropDown")
	TSubclassOf<UX_DropDownMenu> SelectStationClass;
	
// TextBlocks begin
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ErrorTextBlock;
// TextBlocks end

// Buttons begin
	UPROPERTY(meta = (BindWidget))
	UButton* AddStationsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CalculateButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ClearSelectedListButton;
// Buttons end

// Boxes begin
	UPROPERTY(meta = (BindWidget))
	UScrollBox* MainSB;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* OutputProductsVB;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* SelectedStationsVB;
// Boxes end

private:

	UPROPERTY()
	TArray<UX_DropDownMenu*> SelectedStations;

	UPROPERTY()
	TArray<UX_DropDownButton*> DropDownButtons;
};
