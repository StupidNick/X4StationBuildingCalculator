#include "X_ObjectsListWD.h"
#include "X_NameWithAmountWD.h"
#include "X_Types.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"


void UX_ObjectsListWD::CreateList(const TArray<FStationManufacturedInfo>& InManufacturedStations,
	const TArray<FStationManufacturedInfo>& InConsumedStations)
{
	Lines.Empty();
	if (!NameWithAmountClass) return;

	for (const auto Station : InManufacturedStations)
	{
		CreateNewLine(Station.StationName, Station.StationsNumber, Station.ObjectsNumber, FLinearColor::Green);
	}
	for (const auto Station : InConsumedStations)
	{
		CreateNewLine(Station.StationName, Station.StationsNumber, Station.ObjectsNumber, FLinearColor::Red);
	}
}

void UX_ObjectsListWD::CreateList(const TArray<FStationWorkforceInfo>& InManufacturedStations,
	const TArray<FStationWorkforceInfo>& InConsumedStations)
{
	Lines.Empty();
	if (!NameWithAmountClass) return;

	for (const auto Station : InManufacturedStations)
	{
		CreateNewLine(Station.StationName, Station.StationsNumber, Station.WorkforceNumber, FLinearColor::Green);
	}
	for (const auto Station : InConsumedStations)
	{
		CreateNewLine(Station.StationName, Station.StationsNumber, Station.WorkforceNumber, FLinearColor::Red);
	}
}

void UX_ObjectsListWD::CreateListForProductsCost(const TArray<FProductCostInfo>& InCostInfo)
{
	Lines.Empty();

	if (InCostInfo.IsEmpty()) return;
	
	if (InCostInfo[0].Cost > 0)
	{
		for (const auto Product : InCostInfo)
		{
			CreateNewLine(Product.Name, Product.Numbers, Product.Cost, FLinearColor::Blue);
		}
	}
	else
	{
		for (const auto Product : InCostInfo)
		{
			CreateNewLine(Product.Name, Product.Numbers, Product.Cost, FLinearColor::Red);
		}
	}
}

void UX_ObjectsListWD::CreateListForResourcesCost(const TArray<FProductCostInfo>& InResources)
{
	Lines.Empty();

	for (auto Resource : InResources)
	{
		CreateNewLine(Resource.Name, Resource.Numbers, Resource.Cost, FLinearColor::White);
	}
}

void UX_ObjectsListWD::CreateNewLine(const FText& InName, const int32 InNumbers, const int32 InOtherNumber, const FLinearColor& InTextColor)
{
	UX_NameWithAmountWD* NewLine = CreateWidget<UX_NameWithAmountWD>(GetWorld(), NameWithAmountClass);
	if (!NewLine) return;

	FString Name = FString::FromInt(InNumbers);
	Name.Append(" x ");
	Name.Append(InName.ToString());

	NewLine->SetTextColor(InTextColor);
	NewLine->SetInfo(FText::FromString(Name), InOtherNumber);

	VerticalBox->AddChild(NewLine);
	Lines.Add(NewLine);
}
