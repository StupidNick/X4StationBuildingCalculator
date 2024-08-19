#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Service/X_Types.h"
#include "X_MainWidget.generated.h"


struct FNameAndNumbers;
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

	void SetStationsAndCount(TArray<FNameAndNumbers> InStations);
	void ClearSelectedStationsList();
	void SetResult(FResult InResult);

	void PrintError(FText InText) const;
	void PrintError(FString InText) const;

private:
	UFUNCTION()
	void OnAddButtonClicked();
	UFUNCTION()
	void OnCalculateButtonClicked();
	UFUNCTION()
	void OnClearSelectedListButtonClicked();

	FString GetStringFromNamesAndNumbers(TArray<FNameAndNumbers> InStations);

public:

	FNameIntDelegate OnAddStationsButtonClicked;
	FSimpleDelegate OnCalculateButtonClickedEvent;
	FSimpleDelegate OnClearSelectedListButtonClickedEvent;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UX_DropDownMenu* StationsList;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UEditableText* StationsNumber; // Replace to custom editable text block

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* SelectedStationsList;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* OutProducts;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* OutStations;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* ErrorTextBlock;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* AddStationsButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* CalculateButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* ClearSelectedListButton;
};
