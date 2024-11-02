#include "X_ObjectsListWD.h"
#include "X_NameWithAmountWD.h"
#include "X_ObjectsDA.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"


void UX_ObjectsListWD::CreateList(const TArray<FStationManufacturedInfo>& InManufacturedStations, const TArray<FStationManufacturedInfo>& InConsumedStations)
{
	Lines.Empty();
	
	CreateNewLine(InManufacturedStations, true);
	CreateNewLine(InConsumedStations, false);
}

void UX_ObjectsListWD::CreateNewLine(const TArray<FStationManufacturedInfo>& InStations, bool bIsPositive)
{
	if (InStations.IsEmpty()) return;
	
	for (const auto Station : InStations)
	{
		UX_NameWithAmountWD* NewLine = CreateWidget<UX_NameWithAmountWD>(GetWorld(), NameWithAmountClass);
		if (!NewLine) return;

		FString Name = FString::FromInt(Station.StationsNumber);
		Name.Append(" x ");
		Name.Append(Station.StationName.ToString());

		if (bIsPositive)
		{
			NewLine->SetTextColor(FLinearColor(0.0f, 1.0f, 0.0f));
			NewLine->SetInfo(FText::FromString(Name), Station.ObjectsNumber);
		}
		else
		{
			NewLine->SetTextColor(FLinearColor(1.0f, 0.0f, 0.0f));
			NewLine->SetInfo(FText::FromString(Name), -Station.ObjectsNumber);
		}

		VerticalBox->AddChild(NewLine);
		Lines.Add(NewLine);
	}
}
