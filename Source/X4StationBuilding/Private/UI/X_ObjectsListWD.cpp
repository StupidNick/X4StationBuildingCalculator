#include "X_ObjectsListWD.h"
#include "X_NameWithAmountWD.h"
#include "X_ObjectsDA.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Styling/SlateColor.h"


void UX_ObjectsListWD::CreateList(const TArray<FStationManufacturedInfo>& InManufacturedStations, const TArray<FStationManufacturedInfo>& InConsumedStations)
{
	Lines.Empty();
	
	CreateNewLine(InManufacturedStations);
	CreateNewLine(InConsumedStations);
}

void UX_ObjectsListWD::CreateNewLine(const TArray<FStationManufacturedInfo>& InStations)
{
	for (const auto Station : InStations)
	{
		UX_NameWithAmountWD* NewLine = CreateWidget<UX_NameWithAmountWD>(GetWorld(), NameWithAmountClass);
		if (!NewLine) return;

		NewLine->NameTextBlock->SetColorAndOpacity(FLinearColor(0.0f, 1.0f, 0.0f));

		FString Name = FString::FromInt(Station.StationsNumber);
		Name.Append(" ");
		Name.Append(Station.StationName.ToString());
		
		NewLine->SetInfo(FText::FromString(Name), Station.ObjectsNumber);

		VerticalBox->AddChild(NewLine);
		Lines.Add(NewLine);
	}
}
