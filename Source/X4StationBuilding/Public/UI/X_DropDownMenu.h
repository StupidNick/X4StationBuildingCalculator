#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Service/X_Types.h"
#include "X_DropDownMenu.generated.h"


class UX_CustomEditableText;
class UTextBlock;
class UX_StationsList;
class UButton;
class UMenuAnchor;
class UXDA_Stations;

UCLASS()
class X4STATIONBUILDING_API UX_DropDownMenu : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DELEGATE_ThreeParams(FMenuDestroyed, UX_DropDownMenu*, FText, int32)

public:

	UFUNCTION()
	void OnCountChanged(int32 InCount);

	void SetupSelectedStationAndCount(const FText& InStationName, const int32 InNums);

private:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnStationSelected(const FText& InText);

	UFUNCTION()
	void OpenMenu();
	UFUNCTION()
	void DestroyMenu();
	UFUNCTION()
	UUserWidget* OnMenuOpen();

public:

	FTextInt32Delegate OnStationSelectedEvent;
	FTextInt32Delegate OnStationRemovedEvent;
	FChangeStationsCountDelegate OnStationCountChangedEvent;
	FMenuDestroyed OnObjectDestroyedEvent;

	UPROPERTY(EditAnywhere)
	UXDA_Stations* StationsDA;

	UPROPERTY(meta = (BindWidget))
	UMenuAnchor* MenuAnchor;

	UPROPERTY(meta = (BindWidget))
	UButton* OpenButton;

	UPROPERTY(meta = (BindWidget))
	UButton* DestroyButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ButtonTextBlock;

	UPROPERTY(meta = (BindWidget))
	UX_CustomEditableText* CountTextBlock;

	UPROPERTY(EditAnywhere)
	FText ButtonText;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UX_StationsList> ListClass;

private:

	UPROPERTY()
	UX_StationsList* List;

	FText CurrentSelectedStation = FText::FromString("None");
	int32 CurrentStationsCount = 0;
};
