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

	if (!CalculateButton || !AddStationsButton || !ClearSelectedListButton) return;
	
	CalculateButton->OnClicked.AddDynamic(this, &UX_MainWidget::OnCalculateButtonClicked);
	AddStationsButton->OnClicked.AddDynamic(this, &UX_MainWidget::OnAddButtonClicked);
	ClearSelectedListButton->OnClicked.AddDynamic(this, &UX_MainWidget::OnClearSelectedListButtonClicked);

	OnAddButtonClicked();
}

void UX_MainWidget::OnAddButtonClicked()
{
	if (!SelectStationClass || !SelectedStationsVB) return;

	UX_DropDownMenu* NewLine = CreateWidget<UX_DropDownMenu>(GetWorld(), SelectStationClass);
	if (!NewLine) return;

	NewLine->OnStationSelected.BindLambda([&](const FText& InStationName, const int32 InStationsCount)
	{
		AddNewStationEvent.ExecuteIfBound(InStationName, InStationsCount);
	});
	NewLine->OnObjectDestroyed.BindLambda([&](UX_DropDownMenu* InMenu, FText InSelectedStation, int32 InSelectedStationsCount)
	{
		SelectedStations.Remove(InMenu);
		InMenu->RemoveFromParent();
		InMenu->Destruct();
	});
	NewLine->OnStationRemoved.BindLambda([&](const FText& InStationName, const int32 InNums)
	{
		RemoveStationEvent.ExecuteIfBound(InStationName, InNums);
	});
	NewLine->OnStationCountChanged.BindLambda([&](const FText& InStationName, const int32 InOldCount, const int32 InNewCount)
	{
		ChangeStationsCountEvent.ExecuteIfBound(InStationName, InOldCount, InNewCount);
	});

	NewLine->SetPadding(DropDownButtonsPadding);

	SelectedStationsVB->AddChild(NewLine);
	SelectedStations.Add(NewLine);
}

void UX_MainWidget::OnCalculateButtonClicked()
{
	OnCalculateButtonClickedEvent.ExecuteIfBound();
}

void UX_MainWidget::OnClearSelectedListButtonClicked()
{
	OnClearSelectedListButtonClickedEvent.ExecuteIfBound();
}

FString UX_MainWidget::GetStringFromNamesAndNumbers(TArray<FObjectInfo>& InStations)
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

void UX_MainWidget::SetStationsAndCount(TArray<FObjectInfo>& InStations)
{
	// SelectedStationsList->SetText(FText::FromString(GetStringFromNamesAndNumbers(InStations)));
}

void UX_MainWidget::ClearSelectedStationsList()
{
	// if (!SelectedStationsList) return;
	//
	// SelectedStationsList->SetText(FText::FromString(""));
}

void UX_MainWidget::SetResult(FResult& InResult)
{
	if (!MainSB || !DropDownButtonClass) return;

	ClearResults();
	DropDownButtons.Empty();
	
	for (const auto Product : InResult.ResultProducts)
	{
		UX_DropDownButton* Button = CreateWidget<UX_DropDownButton>(GetWorld(), DropDownButtonClass);
		if (!Button) return;

		Button->InitializeWidget(Product.Name, InResult);
		Button->SetPadding(DropDownButtonsPadding);
		OutputProductsVB->AddChild(Button);
		
		DropDownButtons.Add(Button);
	}
}

void UX_MainWidget::ClearResults()
{
	for (const auto Button : DropDownButtons)
	{
		Button->RemoveFromParent();
		Button->Destruct();
	}
}

void UX_MainWidget::PrintError(const FText& InText) const
{
	ErrorTextBlock->SetText(InText);
}

void UX_MainWidget::PrintError(const FString& InText) const
{
	ErrorTextBlock->SetText(FText::FromString(InText));
}
