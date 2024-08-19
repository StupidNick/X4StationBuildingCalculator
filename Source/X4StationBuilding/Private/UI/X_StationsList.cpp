#include "UI/X_StationsList.h"
#include "UI/X_ButtonWithText.h"
#include "XDA_Stations.h"
#include "Components/VerticalBox.h"


void UX_StationsList::CreateList(UXDA_Stations* InStationsDA)
{
	if (!InStationsDA) return;

	for (const auto Station : InStationsDA->Stations)
	{
		UX_ButtonWithText* NewButton = CreateWidget<UX_ButtonWithText>(GetWorld(), ButtonClass);
		if (!NewButton) return;

		NewButton->SetText(Station.StationName);
		NewButton->OnButtonPressedEvent.BindLambda([&](FText InText)
		{
			OnStationSelected.ExecuteIfBound(InText);
		});
		VerticalBox->AddChild(NewButton);
		Buttons.Add(NewButton);
	}
}
