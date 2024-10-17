#include "X_DropDownButtonWD.h"
#include "X_DropDownMenu.h"
#include "X_ObjectsListWD.h"
#include "Components/Button.h"
#include "Components/MenuAnchor.h"



void UX_DropDownButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!OpenButton || !MenuAnchor) return;
	
	MenuAnchor->OnGetUserMenuContentEvent.BindDynamic(this, &UX_DropDownButton::OnMenuOpen);
	OpenButton->OnClicked.AddDynamic(this, &UX_DropDownButton::OpenMenu);
}

void UX_DropDownButton::Initialize(FName InName, FResult& InResult)
{
	if (!ButtonTextBlock) return;

	FObjectInfo* ResultObjects;
	FObjectInfo* NecessaryObjects;
	if (!InResult.FindResultProductsByName(InName, ResultObjects)) return;
	if (InResult.FindNecessaryProductsByName(InName, NecessaryObjects))
	{
		
	}
	
	List = CreateWidget<UX_ObjectsListWD>(GetWorld(), ListClass);
	if (!List) return;

	
	
	List->CreateList(StationsDA);
	List->OnStationSelected.BindLambda([&](FText InText)
	{
		MenuAnchor->Close();
		ButtonTextBlock->SetText(InText);
		CurrentSelectedStation = InText;
		OnStationSelected.ExecuteIfBound(InText);
	});
	
	ButtonTextBlock->SetText(ButtonText);
}

void UX_DropDownButton::OpenMenu()
{
}

UUserWidget* UX_DropDownButton::OnMenuOpen()
{
}
