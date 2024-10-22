#include "UI/X_MainWidget.h"

#include "X_DropDownButtonWD.h"
#include "X_DropDownMenu.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"


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

FString UX_MainWidget::GetStringFromNamesAndNumbers(TArray<FObjectInfo> InStations)
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

void UX_MainWidget::SetStationsAndCount(TArray<FObjectInfo> InStations)
{
	SelectedStationsList->SetText(FText::FromString(GetStringFromNamesAndNumbers(InStations)));
}

void UX_MainWidget::ClearSelectedStationsList()
{
	if (!SelectedStationsList) return;

	SelectedStationsList->SetText(FText::FromString(""));
}

void UX_MainWidget::SetResult(FResult& InResult)
{
	// NecessaryProducts->SetText(FText::FromString(GetStringFromNamesAndNumbers(InResult.NecessaryProducts)));
	// OutStations->SetText(FText::FromString(GetStringFromNamesAndNumbers(InResult.ResultStations)));
	// ResultProducts->SetText(FText::FromString(GetStringFromNamesAndNumbers(InResult.ResultProducts)));

	if (!OutputProductsVB || !DropDownButtonClass) return;

	DropDownButtons.Empty();
	for (auto Product : InResult.ResultProducts)
	{
		UX_DropDownButton* Button = CreateWidget<UX_DropDownButton>(GetWorld(), DropDownButtonClass);
		if (!Button) return;

		Button->InitializeWidget(Product.Name, InResult);
		OutputProductsVB->AddChild(Button);
		
		DropDownButtons.Add(Button);
	}
	// TODO make result info with drop down buttons
}

void UX_MainWidget::PrintError(FText InText) const
{
	ErrorTextBlock->SetText(InText);
}

void UX_MainWidget::PrintError(FString InText) const
{
	ErrorTextBlock->SetText(FText::FromString(InText));
}
