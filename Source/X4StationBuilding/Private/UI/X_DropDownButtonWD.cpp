#include "X_DropDownButtonWD.h"
#include "X_DropDownMenu.h"
#include "X_ObjectsListWD.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"


void UX_DropDownButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!OpenButton) return;
	
	OpenButton->OnClicked.AddDynamic(this, &UX_DropDownButton::OpenMenu);
}

void UX_DropDownButton::InitializeWidget(FText InName, FResult& InResult)
{
	if (!NameTextBlock || !AmountTextBlock || !DetailsVerticalBox) return;

	FObjectInfo* ResultObjects;
	FObjectInfo* NecessaryObjects;
	if (!InResult.FindResultProductsByName(InName, ResultObjects)) return;

	// UE_LOG(LogTemp, Warning, TEXT("Result products: %s x%i"), *ResultObjects->Name.ToString(), ResultObjects->Numbers);
	
	int32 ResultProductNumber = ResultObjects->Numbers;
	if (InResult.FindNecessaryProductsByName(InName, NecessaryObjects))
	{
		ResultProductNumber -= NecessaryObjects->Numbers;
		// UE_LOG(LogTemp, Warning, TEXT("Necessary products: %s x%i"), *NecessaryObjects->Name.ToString(), NecessaryObjects->Numbers);
		// UE_LOG(LogTemp, Warning, TEXT("Result: %i"), ResultObjects->Numbers - NecessaryObjects->Numbers);
	}
	NameTextBlock->SetText(InName);
	AmountTextBlock->SetText(FText::AsNumber(ResultProductNumber));
	if (ResultProductNumber > 0)
	{
		SetTextColor(FLinearColor(0.0f, 1.0f, 0.0f));
	}
	else if (ResultProductNumber < 0)
	{
		SetTextColor(FLinearColor(1.0f, 0.0f, 0.0f));
	}
	
	List = CreateWidget<UX_ObjectsListWD>(GetWorld(), ListClass);
	if (!List) return;
	
	List->CreateList(InResult.FindAllManufacturedStationsByProductName(InName), InResult.FindAllConsumedStationsByProductName(InName));
	
	List->SetVisibility(ESlateVisibility::Collapsed);
	DetailsVerticalBox->AddChild(List);
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
