#include "UI/X_StationsList.h"
#include "UI/X_ButtonWithText.h"
#include "XDA_Stations.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"


void UX_StationsList::CreateList(UXDA_Stations* InStationsDA)
{
	if (!InStationsDA || !VerticalBox) return;

	int32 CurrentStationID = -1;
	for (int i = 0; i < InStationsDA->Stations.Num(); i++)
	{
		if (CurrentStationID != InStationsDA->Stations[i].SortID)
		{
			CurrentStationID = InStationsDA->Stations[i].SortID;
			CreateCategory(InStationsDA, CurrentStationID);
		}
		
		UX_ButtonWithText* NewButton = CreateWidget<UX_ButtonWithText>(GetWorld(), ButtonClass);
		if (!NewButton) return;

		NewButton->SetText(InStationsDA->Stations[i].StationName);
		NewButton->OnButtonPressedEvent.BindLambda([&](FText InText)
		{
			OnStationSelected.ExecuteIfBound(InText);
		});
		VerticalBox->AddChild(NewButton);
		Buttons.Add(NewButton);
	}
}

void UX_StationsList::CreateCategory(UXDA_Stations* InStationsDA, const int32 InCurrentCategoryID) const
{
	UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>();
	if (!Text) return;

	const FText* Category = InStationsDA->Categories.Find(InCurrentCategoryID);
	if (!Category) return;
	
	Text->SetText(*Category);
	VerticalBox->AddChild(Text);
		
	if (UBorder* Border = WidgetTree->ConstructWidget<UBorder>())
	{
		VerticalBox->AddChild(Border);
	}
}
