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
	List->OnStationSelected.BindUObject(this, &UX_DropDownMenu::OnStationSelected);
	
	ButtonTextBlock->SetText(ButtonText);
}

void UX_DropDownMenu::OnStationSelected(const FText& InText)
{
	MenuAnchor->Close();
	ButtonTextBlock->SetText(InText);
	if (CurrentSelectedStation.ToString() != "None")
	{
		OnStationRemovedEvent.ExecuteIfBound(CurrentSelectedStation, CurrentStationsCount);
	}
	if (CurrentSelectedStation.ToString() != InText.ToString())
	{
		OnStationSelectedEvent.ExecuteIfBound(InText, CurrentStationsCount);
		CurrentSelectedStation = InText;
	}
}

void UX_DropDownMenu::OnCountChanged(int32 InCount)
{
	if (CurrentSelectedStation.ToString() != "None" && CurrentStationsCount != InCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("Send message then station count changed: from %i to %i"), CurrentStationsCount, InCount);
		OnStationCountChangedEvent.ExecuteIfBound(CurrentSelectedStation, CurrentStationsCount, InCount);
	}
	CurrentStationsCount = InCount;
}

void UX_DropDownMenu::SetupSelectedStationAndCount(const FText& InStationName, const int32 InNums)
{
	ButtonTextBlock->SetText(InStationName); // TODO check info before using
	CurrentSelectedStation = InStationName;
	// OnStationSelected(InStationName);
	CountTextBlock->SetText(FText::AsNumber(InNums));
}

FObjectInfo UX_DropDownMenu::GetStationInfo() const
{
	return FObjectInfo(CurrentSelectedStation, CurrentStationsCount);
}

void UX_DropDownMenu::OpenMenu()
{
	if (!MenuAnchor || MenuAnchor->IsOpen()) return;

	MenuAnchor->Open(true);
}

void UX_DropDownMenu::DestroyMenu()
{
	OnStationRemovedEvent.ExecuteIfBound(CurrentSelectedStation, CurrentStationsCount);
	OnObjectDestroyedEvent.ExecuteIfBound(this, CurrentSelectedStation, CurrentStationsCount);
}

UUserWidget* UX_DropDownMenu::OnMenuOpen()
{
	return List;
}
