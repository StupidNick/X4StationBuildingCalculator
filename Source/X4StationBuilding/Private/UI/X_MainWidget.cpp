#include "UI/X_MainWidget.h"
#include "X_DropDownButtonWD.h"
#include "X_DropDownMenu.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"


void UX_MainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!AutofillButton || !AddStationsButton || !ClearSelectedListButton) return;
	
	AutofillButton->OnClicked.AddDynamic(this, &UX_MainWidget::OnAutofillButtonClicked);
	AddStationsButton->OnClicked.AddDynamic(this, &UX_MainWidget::OnAddButtonClicked);
	ClearSelectedListButton->OnClicked.AddDynamic(this, &UX_MainWidget::OnClearSelectedListButtonClicked);

	OnAddButtonClicked();
}

void UX_MainWidget::OnAddButtonClicked()
{
	AddStationLine();
}

void UX_MainWidget::OnAutofillButtonClicked()
{
	// if (LastResult.IsEmpty()) return;

	OnAutofillButtonClickedEvent.ExecuteIfBound();

	// for (auto Station : LastResult.ResultStations)
	// {
	// 	const auto StationLine = AddStationLine();
	// 	if (!StationLine) continue;
	//
	// 	StationLine->SetupSelectedStationAndCount(Station.Name, Station.Numbers);
	// }
}

void UX_MainWidget::OnClearSelectedListButtonClicked()
{
	OnClearSelectedListButtonClickedEvent.ExecuteIfBound();
}

void UX_MainWidget::ClearSelectedStationsList()
{
	if (SelectedStations.IsEmpty()) return;

	for (const auto Station : SelectedStations)
	{
		Station->RemoveFromParent();
		Station->Destruct();
	}
	SelectedStations.Empty();
	ClearResults();
}

void UX_MainWidget::SetResult(FResult& InResult)
{
	if (!MainSB || !DropDownButtonClass) return;

	ClearResults();
	DropDownButtons.Empty();
	
	for (const auto Product : InResult.AllProducts)
	{
		UX_DropDownButton* Button = CreateWidget<UX_DropDownButton>(GetWorld(), DropDownButtonClass);
		if (!Button) continue;

		Button->InitializeWidget(Product, InResult);
		Button->SetPadding(DropDownButtonsPadding);
		OutputProductsVB->AddChild(Button);
		
		DropDownButtons.Add(Button);
	}
	LastResult = InResult;
}

void UX_MainWidget::ClearResults()
{
	for (const auto Button : DropDownButtons)
	{
		Button->RemoveFromParent();
		Button->Destruct();
	}
	LastResult.Empty();
}

UX_DropDownMenu* UX_MainWidget::AddStationLine()
{
	if (!SelectStationClass || !SelectedStationsVB) return nullptr;

	UX_DropDownMenu* NewLine = CreateWidget<UX_DropDownMenu>(GetWorld(), SelectStationClass);
	if (!NewLine) return nullptr;

	NewLine->OnStationSelectedEvent.BindLambda([&](const FText& InStationName, const int32 InStationsCount)
	{
		AddNewStationEvent.ExecuteIfBound(InStationName, InStationsCount);
	});
	NewLine->OnObjectDestroyedEvent.BindLambda([&](UX_DropDownMenu* InMenu, FText InSelectedStation, int32 InSelectedStationsCount)
	{
		SelectedStations.Remove(InMenu);
		InMenu->RemoveFromParent();
		InMenu->Destruct();
		if (SelectedStations.IsEmpty())
		{
			ClearResults();
		}
	});
	NewLine->OnStationRemovedEvent.BindLambda([&](const FText& InStationName, const int32 InNums)
	{
		RemoveStationEvent.ExecuteIfBound(InStationName, InNums);
	});
	NewLine->OnStationCountChangedEvent.BindLambda([&](const FText& InStationName, const int32 InOldCount, const int32 InNewCount)
	{
		ChangeStationsCountEvent.ExecuteIfBound(InStationName, InOldCount, InNewCount);
	});

	NewLine->SetPadding(DropDownButtonsPadding);

	SelectedStationsVB->AddChild(NewLine);
	SelectedStations.Add(NewLine);
	return NewLine;
}

void UX_MainWidget::PrintError(const FText& InText) const
{
	ErrorTextBlock->SetText(InText);
}

void UX_MainWidget::PrintError(const FString& InText) const
{
	ErrorTextBlock->SetText(FText::FromString(InText));
}
