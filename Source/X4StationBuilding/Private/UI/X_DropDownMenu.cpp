#include "UI/X_DropDownMenu.h"
#include "X_StationsList.h"
#include "Components/Button.h"
#include "Components/MenuAnchor.h"
#include "Components/TextBlock.h"


void UX_DropDownMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!OpenButton || !MenuAnchor || !ButtonTextBlock) return;
	
	MenuAnchor->OnGetUserMenuContentEvent.BindDynamic(this, &UX_DropDownMenu::OnMenuOpen);
	
	OpenButton->OnClicked.AddDynamic(this, &UX_DropDownMenu::OpenMenu);
	
	List = CreateWidget<UX_StationsList>(GetWorld(), ListClass);
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

void UX_DropDownMenu::OpenMenu()
{
	if (!MenuAnchor || MenuAnchor->IsOpen()) return;

	MenuAnchor->Open(true);
}

UUserWidget* UX_DropDownMenu::OnMenuOpen()
{
	return List;
}
