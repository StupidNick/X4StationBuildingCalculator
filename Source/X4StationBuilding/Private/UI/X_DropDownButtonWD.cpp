#include "X_DropDownButtonWD.h"
#include "X_DropDownMenu.h"
#include "X_ObjectsListWD.h"
#include "Components/Button.h"
#include "Components/MenuAnchor.h"
#include "Components/TextBlock.h"


void UX_DropDownButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!OpenButton || !MenuAnchor) return;
	
	MenuAnchor->OnGetUserMenuContentEvent.BindDynamic(this, &UX_DropDownButton::OnMenuOpen);
	OpenButton->OnClicked.AddDynamic(this, &UX_DropDownButton::OpenMenu);
}

void UX_DropDownButton::InitializeWidget(FName InName, FResult& InResult)
{
	if (!NameTextBlock || !AmountTextBlock) return;

	FObjectInfo* ResultObjects;
	FObjectInfo* NecessaryObjects;
	if (!InResult.FindResultProductsByName(InName, ResultObjects)) return;
	InResult.FindNecessaryProductsByName(InName, NecessaryObjects);

	const int32 ResultProductNumber = ResultObjects->Numbers - NecessaryObjects->Numbers; 
	NameTextBlock->SetText(FText::FromName(InName));
	AmountTextBlock->SetText(FText::AsNumber(ResultProductNumber));
	
	List = CreateWidget<UX_ObjectsListWD>(GetWorld(), ListClass);
	if (!List) return;
	
	List->CreateList(InResult.FindAllManufacturedStationsByProductName(InName), InResult.FindAllConsumedStationsByProductName(InName));
}

void UX_DropDownButton::OpenMenu()
{
	if (!MenuAnchor) return;

	if (MenuAnchor->IsOpen())
	{
		MenuAnchor->Close();
		return;
	}
	MenuAnchor->Open(true);
}

UUserWidget* UX_DropDownButton::OnMenuOpen()
{
	return List;
}
