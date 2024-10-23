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

	void PrintError(FText InText) const;
	void PrintError(FString InText) const;

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

	FNameIntDelegate OnAddStationsButtonClicked;
	FSimpleDelegate OnCalculateButtonClickedEvent;
	FSimpleDelegate OnClearSelectedListButtonClickedEvent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UX_DropDownButton> DropDownButtonClass;

	UPROPERTY(EditDefaultsOnly)
	FMargin DropDownButtonsPadding;

	UPROPERTY()
	TArray<UX_DropDownButton*> DropDownButtons;
	
	UPROPERTY(meta = (BindWidget))
	UX_DropDownMenu* StationsList;

	UPROPERTY(meta = (BindWidget))
	UEditableText* StationsNumber; // Replace to custom editable text block
	
// TextBlocks begin
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SelectedStationsList;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NecessaryProducts;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ResultProducts;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* OutStations;

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
// Boxes end
};
