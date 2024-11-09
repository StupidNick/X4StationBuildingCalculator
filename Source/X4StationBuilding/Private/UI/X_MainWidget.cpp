#include "UI/X_MainWidget.h"

#include "X_CheckBoxWithText.h"
#include "X_DropDownButtonWD.h"
#include "X_DropDownMenu.h"
#include "X_NameWithAmountWD.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"


void UX_MainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!AutofillButton || !AddStationsButton || !ClearSelectedListButton || !ProvideBasicResourcesCB || !ProvideAllResourcesCB) return;
	
	AutofillButton->OnClicked.AddDynamic(this, &UX_MainWidget::OnAutofillButtonClicked);
	AddStationsButton->OnClicked.AddDynamic(this, &UX_MainWidget::OnAddButtonClicked);
	ClearSelectedListButton->OnClicked.AddDynamic(this, &UX_MainWidget::OnClearSelectedListButtonClicked);

	ProvideBasicResourcesCB->SetText(BasicResourcesCheckBoxName);
	ProvideBasicResourcesCB->OnValueChangedEvent.BindLambda([&](bool InValue)
	{
		OnProvideBasicResourceValueChanged.ExecuteIfBound(InValue);
	});
	ProvideAllResourcesCB->SetText(AllResourcesCheckBoxName);
	ProvideAllResourcesCB->OnValueChangedEvent.BindLambda([&](bool InValue)
	{
		OnProvideAllResourceValueChanged.ExecuteIfBound(InValue);
	});

	OnAddButtonClicked();
}

void UX_MainWidget::OnAddButtonClicked()
{
	AddStationLine();
}

void UX_MainWidget::OnAutofillButtonClicked()
{
	FResult Result;
	TArray<FObjectInfo> Stations;
	for (const auto Station : SelectedStations)
	{
		Stations.Add(Station->GetStationInfo());
		Station->RemoveFromParent();
		Station->Destruct();
	}
	SelectedStations.Empty();

	CalculateStationsEvent.ExecuteIfBound(Stations, Result);
	if (Result.ResultStations.IsEmpty()) return;

	for (auto Station : Result.ResultStations)
	{
		UX_DropDownMenu* NewLine = AddStationLine();
		if (!NewLine) continue;

		NewLine->SetupSelectedStationAndCount(Station.Name, Station.Numbers);
	}

	SetResult(Result);
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
	
	SetProductsInfo(InResult);
	SetWorkforceInfo(InResult);
	SetProductionCostInfo(InResult);
	SetStationCostInfo(InResult);
}

void UX_MainWidget::SetProductsInfo(FResult& InResult)
{
	if (!OutputProductsVB) return;
	
	for (const auto Product : InResult.AllProducts)
	{
		UX_DropDownButton* Button = CreateWidget<UX_DropDownButton>(GetWorld(), DropDownButtonClass);
		if (!Button) continue;

		Button->InitializeWidgetAsProductsInfo(Product, InResult);
		OutputProductsVB->AddChild(Button);
		
		DropDownButtons.Add(Button);
	}
}

void UX_MainWidget::SetWorkforceInfo(const FResult& InResult)
{
	if (!OutputWorkforceVB) return;
	
	UX_DropDownButton* Button = CreateWidget<UX_DropDownButton>(GetWorld(), DropDownButtonClass);
	if (!Button) return;

	Button->InitializeWidgetAsWorkforceInfo(InResult);
	OutputWorkforceVB->AddChild(Button);
	
	DropDownButtons.Add(Button);
}

void UX_MainWidget::SetProductionCostInfo(const FResult& InResult)
{
	if (!OutputProductsVB || !NameWithAmountClass) return;

	CreateResourcesPerHourButton(InResult.ExpensesProducts, InResult.TotalExpensesPerHour);
	CreateResourcesPerHourButton(InResult.ProductionsProducts, InResult.TotalProductionPerHour);

	UX_NameWithAmountWD* ProfitLine = CreateWidget<UX_NameWithAmountWD>(GetWorld(), NameWithAmountClass);
	if (!ProfitLine) return;

	ProfitLine->SetInfo(ProfitName, InResult.TotalProfitPerHour);
	ProfitLine->SetTextColor(FLinearColor::Green);
	ProfitLine->SetPadding(FMargin(10.f, 0.f, 0.f, 0.f));
	OutputProductsCostVB->AddChild(ProfitLine);

	LinesWithAmount.Add(ProfitLine);
}

void UX_MainWidget::SetStationCostInfo(const FResult& InResult)
{
	if (!OutputStationCostVB) return;

	for (auto StationBuildingInfo : InResult.StationsBuildingCostInfo)
	{
		UX_DropDownButton* Button = CreateWidget<UX_DropDownButton>(GetWorld(), DropDownButtonClass);
		if (!Button) return;

		Button->InitializeWidgetAsStationCostsInfo(StationBuildingInfo);
		OutputStationCostVB->AddChild(Button);
	
		DropDownButtons.Add(Button);
	}
	UX_NameWithAmountWD* TotalLine = CreateWidget<UX_NameWithAmountWD>(GetWorld(), NameWithAmountClass);
	if (!TotalLine) return;

	TotalLine->SetInfo(TotalCostStationName, InResult.StationBuildingTotalCost);
	OutputStationCostVB->AddChild(TotalLine);

	LinesWithAmount.Add(TotalLine);
}

void UX_MainWidget::CreateResourcesPerHourButton(const TArray<FProductCostInfo>& InInfo, const int32 InTotalCost)
{
	UX_DropDownButton* Button = CreateWidget<UX_DropDownButton>(GetWorld(), DropDownButtonClass);
	if (!Button) return;

	Button->InitializeWidgetAsResultCostsInfo(InInfo, InTotalCost);
	OutputWorkforceVB->AddChild(Button);
	
	DropDownButtons.Add(Button);
}

void UX_MainWidget::ClearResults()
{
	for (const auto Button : DropDownButtons)
	{
		Button->RemoveFromParent();
		Button->Destruct();
	}
	for (const auto Line : LinesWithAmount)
	{
		Line->RemoveFromParent();
		Line->Destruct();
	}
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
