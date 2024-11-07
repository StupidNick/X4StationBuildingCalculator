#include "X_DropDownButtonWD.h"
#include "X_DropDownMenu.h"
#include "X_ObjectsListWD.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "X_Types.h"


void UX_DropDownButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!OpenButton) return;
	
	OpenButton->OnClicked.AddDynamic(this, &UX_DropDownButton::OpenMenu);
}

void UX_DropDownButton::InitializeWidgetAsProductsInfo(const FText& InName, FResult& InResult)
{
	if (!NameTextBlock || !AmountTextBlock || !DetailsVerticalBox) return;

	FObjectInfo *ResultObjects, *NecessaryObjects;
	int32 ResultProductNumber = 0;
	if (InResult.FindResultProductsByName(InName, ResultObjects))
	{
		ResultProductNumber = ResultObjects->Numbers;
	}
	
	if (InResult.FindNecessaryProductsByName(InName, NecessaryObjects))
	{
		ResultProductNumber -= NecessaryObjects->Numbers;
	}
	NameTextBlock->SetText(InName);
	AmountTextBlock->SetText(FText::AsNumber(ResultProductNumber));
	if (ResultProductNumber > 0)
	{
		SetTextColor(FLinearColor::Green);
	}
	else if (ResultProductNumber < 0)
	{
		SetTextColor(FLinearColor::Red);
	}
	
	List = CreateWidget<UX_ObjectsListWD>(GetWorld(), ListClass);
	if (!List) return;
	
	List->CreateList(InResult.FindAllManufacturedStationsByProductName(InName), InResult.FindAllConsumedStationsByProductName(InName));
	
	List->SetVisibility(ESlateVisibility::Collapsed);
	DetailsVerticalBox->AddChild(List);
}

void UX_DropDownButton::InitializeWidgetAsWorkforceInfo(const FResult& InResult)
{
	const int32 ResultWorkforce = InResult.TotalAvailableWorkforceNumber - InResult.TotalNeededWorkforceNumber;
	TArray<FStationWorkforceInfo> PositiveWorkforce, NegativeWorkforce;
	for (const auto Workforce : InResult.WorkforceInfo)
	{
		if (Workforce.WorkforceNumber > 0)
		{
			PositiveWorkforce.Add(Workforce);
		}
		else
		{
			NegativeWorkforce.Add(Workforce);
		}
	}

	List = CreateWidget<UX_ObjectsListWD>(GetWorld(), ListClass);
	if (!List) return;
	
	List->CreateList(PositiveWorkforce, NegativeWorkforce);
	
	List->SetVisibility(ESlateVisibility::Collapsed);
	DetailsVerticalBox->AddChild(List);

	if (ResultWorkforce > 0)
	{
		SetTextColor(FLinearColor::Green);
	}
	else if (ResultWorkforce < 0)
	{
		SetTextColor(FLinearColor::Red);
	}

	NameTextBlock->SetText(WorkforceName);
	AmountTextBlock->SetText(FText::AsNumber(ResultWorkforce));
}

void UX_DropDownButton::InitializeWidgetAsResultCostsInfo(const TArray<FProductCostInfo>& InCostInfo, const int32 InTotalCost)
{
	if (InTotalCost > 0)
	{
		SetTextColor(FLinearColor::Blue);
		NameTextBlock->SetText(ProductionName);
	}
	else if (InTotalCost <= 0)
	{
		SetTextColor(FLinearColor::Red);
		NameTextBlock->SetText(ExpensesName);
	}
	
	List = CreateWidget<UX_ObjectsListWD>(GetWorld(), ListClass);
	if (!List) return;
	
	List->CreateListForCosts(InCostInfo);
	
	List->SetVisibility(ESlateVisibility::Collapsed);
	DetailsVerticalBox->AddChild(List);
	
	AmountTextBlock->SetText(FText::AsNumber(InTotalCost));
}

void UX_DropDownButton::InitializeWidgetAsStationCostsInfo(const TArray<FStationBuildingInfo>& InCostInfo)
{
	for (const auto& Info : InCostInfo)
	{
		List = CreateWidget<UX_ObjectsListWD>(GetWorld(), ListClass);
		if (!List) return;

		
	}
}

void UX_DropDownButton::OpenMenu()
{
	if (!List) return;

	if (bIsOpen)
	{
		List->SetVisibility(ESlateVisibility::Collapsed);
		bIsOpen = false;
		return;
	}
	List->SetVisibility(ESlateVisibility::Visible);
	bIsOpen = true;
}

void UX_DropDownButton::SetTextColor(const FLinearColor& InColor) const
{
	if (!NameTextBlock || !AmountTextBlock) return;

	NameTextBlock->SetColorAndOpacity(InColor);
	AmountTextBlock->SetColorAndOpacity(InColor);
}
