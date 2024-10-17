#include "X_ObjectsListWD.h"
#include "X_NameWithAmountWD.h"
#include "X_ObjectsDA.h"


void UX_ObjectsListWD::CreateList(FResult& InResult)
{
	if (InResult.NecessaryProducts.IsEmpty() || InResult.ResultProducts.IsEmpty()) return;

	for (const auto Object : InResult.ResultProducts)
	{
		UX_NameWithAmountWD* NewLine = CreateWidget<UX_NameWithAmountWD>(GetWorld(), NameWithAmountClass);
		if (!NewLine) return;

		FObjectInfo* NecessaryProducts;
		if (InResult.FindNecessaryProductsByName(Object.Name, NecessaryProducts))
		{
			const int32 Amount = Object.Numbers - NecessaryProducts->Numbers;
		}
		else
		{
			
		}

		NewLine->SetInfo(Object.StationName);
		NewLine->OnButtonPressedEvent.BindLambda([&](FText InText)
		{
			OnStationSelected.ExecuteIfBound(InText);
		});
		VerticalBox->AddChild(NewLine);
		Buttons.Add(NewLine);
	}
}
