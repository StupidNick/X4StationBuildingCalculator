#include "UI/X_MainWidget.h"

#include "X_DropDownMenu.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"



void UX_MainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!CalculateButton || !AddStationsButton || !ClearSelectedListButton || !StationsList) return;
	
	CalculateButton->OnClicked.AddDynamic(this, &UX_MainWidget::OnCalculateButtonClicked);
	AddStationsButton->OnClicked.AddDynamic(this, &UX_MainWidget::OnAddButtonClicked);
	ClearSelectedListButton->OnClicked.AddDynamic(this, &UX_MainWidget::OnClearSelectedListButtonClicked);
}

void UX_MainWidget::OnAddButtonClicked()
{
	if (StationsList->CurrentSelectedStation.ToString() == "None")
	{
		PrintError("Select some station first");
		return;
	}
	if (StationsNumber->GetText().ToString().Len() <= 0)
	{
		PrintError("Enter station count first");
		return;
	}
	OnAddStationsButtonClicked.ExecuteIfBound(*StationsList->CurrentSelectedStation.ToString(), FCString::Atoi(*StationsNumber->GetText().ToString()));
}

void UX_MainWidget::OnCalculateButtonClicked()
{
	OnCalculateButtonClickedEvent.ExecuteIfBound();
}

void UX_MainWidget::OnClearSelectedListButtonClicked()
{
	OnClearSelectedListButtonClickedEvent.ExecuteIfBound();
}

FString UX_MainWidget::GetStringFromNamesAndNumbers(TArray<FNameAndNumbers> InStations)
{
	FString Result;
	for (auto Station : InStations)
	{
		Result += Station.Name.ToString();
		Result += " x";
		Result += FString::FromInt(Station.Numbers);
		Result += '\n';
	}
	return Result;
}

void UX_MainWidget::SetStationsAndCount(TArray<FNameAndNumbers> InStations)
{
	SelectedStationsList->SetText(FText::FromString(GetStringFromNamesAndNumbers(InStations)));
}

void UX_MainWidget::ClearSelectedStationsList()
{
	if (!SelectedStationsList) return;

	SelectedStationsList->SetText(FText::FromString(""));
}

void UX_MainWidget::SetResult(FResult InResult)
{
	OutProducts->SetText(FText::FromString(GetStringFromNamesAndNumbers(InResult.ResultProductions)));
	OutStations->SetText(FText::FromString(GetStringFromNamesAndNumbers(InResult.ResultStations)));
}

void UX_MainWidget::PrintError(FText InText) const
{
	ErrorTextBlock->SetText(InText);
}

void UX_MainWidget::PrintError(FString InText) const
{
	ErrorTextBlock->SetText(FText::FromString(InText));
}
