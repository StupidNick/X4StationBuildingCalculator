#include "X_DropDownMenu.h"
#include "X_CustomEditableText.h"
#include "X_StationsList.h"
#include "Components/Button.h"
#include "Components/MenuAnchor.h"
#include "Components/TextBlock.h"



void UX_DropDownMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!OpenButton || !MenuAnchor || !ButtonTextBlock || !CountTextBlock || !DestroyButton) return;
	
	MenuAnchor->OnGetUserMenuContentEvent.BindDynamic(this, &UX_DropDownMenu::OnMenuOpen);
	OpenButton->OnClicked.AddDynamic(this, &UX_DropDownMenu::OpenMenu);
	DestroyButton->OnClicked.AddDynamic(this, &UX_DropDownMenu::DestroyMenu);
	CountTextBlock->OnEditedTextChangedEvent.BindUObject(this, &UX_DropDownMenu::OnCountChanged);

	CountTextBlock->InitializeWidget();
	
	List = CreateWidget<UX_StationsList>(GetWorld(), ListClass);
	if (!List) return;
	
	List->CreateList(StationsDA);
	List->OnStationSelected.BindLambda([&](const FText& InText)
	{
		MenuAnchor->Close();
		ButtonTextBlock->SetText(InText);
		if (CurrentSelectedStation.ToString() != "None")
		{
			OnStationRemoved.ExecuteIfBound(CurrentSelectedStation, CurrentStationsCount);
		}
		UE_LOG(LogTemp, Warning, TEXT("Select station: %s x%i"), *InText.ToString(), CurrentStationsCount);
		if (CurrentSelectedStation.ToString() != InText.ToString())
		{
			OnStationSelected.ExecuteIfBound(InText, CurrentStationsCount);
			CurrentSelectedStation = InText;
		}
	});
	
	ButtonTextBlock->SetText(ButtonText);
}

void UX_DropDownMenu::OnCountChanged(int32 InCount)
{
	if (CurrentSelectedStation.ToString() != "None" && CurrentStationsCount != InCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("Send message then station count changed: from %i to %i"), CurrentStationsCount, InCount);
		OnStationCountChanged.ExecuteIfBound(CurrentSelectedStation, CurrentStationsCount, InCount);
	}
	CurrentStationsCount = InCount;
}

void UX_DropDownMenu::OpenMenu()
{
	if (!MenuAnchor || MenuAnchor->IsOpen()) return;

	MenuAnchor->Open(true);
}

void UX_DropDownMenu::DestroyMenu()
{
	OnStationRemoved.ExecuteIfBound(CurrentSelectedStation, CurrentStationsCount);
	OnObjectDestroyed.ExecuteIfBound(this, CurrentSelectedStation, CurrentStationsCount);
}

UUserWidget* UX_DropDownMenu::OnMenuOpen()
{
	return List;
}
