#include "X_ObjectsListWD.h"
#include "X_NameWithAmountWD.h"
#include "X_Types.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"


void UX_ObjectsListWD::CreateList(const TArray<FStationManufacturedInfo> InManufacturedStations,
	const TArray<FStationManufacturedInfo> InConsumedStations)
{
	Lines.Empty();
	
	CreateNewLine(InManufacturedStations, true);
	CreateNewLine(InConsumedStations, false);
}

void UX_ObjectsListWD::CreateList(const TArray<FStationWorkforceInfo> InManufacturedStations,
	const TArray<FStationWorkforceInfo> InConsumedStations)
{
	Lines.Empty();
	
	CreateNewLine(InManufacturedStations, true);
	CreateNewLine(InConsumedStations, false);
}

void UX_ObjectsListWD::CreateListForCosts(const TArray<FStationManufacturedInfo> InCostInfo)
{
	Lines.Empty();

	if (InCostInfo.IsEmpty()) return;
	
	if (InCostInfo[0].TotalObjectsCost > 0)
	{
		CreateNewLineForCosts(InCostInfo, true);
	}
	else
	{
		CreateNewLineForCosts(InCostInfo, false);
	}
}

void UX_ObjectsListWD::CreateNewLine(const TArray<FStationManufacturedInfo> InStations, bool bIsPositive)
{
	if (InStations.IsEmpty() || !NameWithAmountClass) return;
	
	for (const auto Station : InStations)
	{
		UX_NameWithAmountWD* NewLine = CreateWidget<UX_NameWithAmountWD>(GetWorld(), NameWithAmountClass);
		if (!NewLine) return;

		FString Name = FString::FromInt(Station.StationsNumber);
		Name.Append(" x ");
		Name.Append(Station.StationName.ToString());

		if (bIsPositive)
		{
			NewLine->SetTextColor(FLinearColor::Green);
			NewLine->SetInfo(FText::FromString(Name), Station.ObjectsNumber);
		}
		else
		{
			NewLine->SetTextColor(FLinearColor::Red);
			NewLine->SetInfo(FText::FromString(Name), -Station.ObjectsNumber);
		}

		VerticalBox->AddChild(NewLine);
		Lines.Add(NewLine);
	}
}

void UX_ObjectsListWD::CreateNewLine(const TArray<FStationWorkforceInfo> InStations, bool bIsPositive)
{
	if (InStations.IsEmpty() || !NameWithAmountClass) return;
	
	for (const auto Station : InStations)
	{
		UX_NameWithAmountWD* NewLine = CreateWidget<UX_NameWithAmountWD>(GetWorld(), NameWithAmountClass);
		if (!NewLine) return;

		FString Name = FString::FromInt(Station.StationsNumber);
		Name.Append(" x ");
		Name.Append(Station.StationName.ToString());

		if (bIsPositive)
		{
			NewLine->SetTextColor(FLinearColor::Green);
			NewLine->SetInfo(FText::FromString(Name), Station.WorkforceNumber);
		}
		else
		{
			NewLine->SetTextColor(FLinearColor::Red);
			NewLine->SetInfo(FText::FromString(Name), Station.WorkforceNumber);
		}

		VerticalBox->AddChild(NewLine);
		Lines.Add(NewLine);
	}
}

void UX_ObjectsListWD::CreateNewLineForCosts(const TArray<FStationManufacturedInfo> InProducts, bool bIsPositive)
{
	if (InProducts.IsEmpty() || !NameWithAmountClass) return;
	
	for (const auto Product : InProducts)
	{
		UX_NameWithAmountWD* NewLine = CreateWidget<UX_NameWithAmountWD>(GetWorld(), NameWithAmountClass);
		if (!NewLine) return;

		FString Name = FString::FromInt(Product.ObjectsNumber);
		Name.Append(" x ");
		Name.Append(Product.ObjectName.ToString());

		if (bIsPositive)
		{
			NewLine->SetTextColor(FLinearColor::Blue);
			NewLine->SetInfo(FText::FromString(Name), Product.TotalObjectsCost);
		}
		else
		{
			NewLine->SetTextColor(FLinearColor::Red);
			NewLine->SetInfo(FText::FromString(Name), Product.TotalObjectsCost);
		}

		VerticalBox->AddChild(NewLine);
		Lines.Add(NewLine);
	}
}
