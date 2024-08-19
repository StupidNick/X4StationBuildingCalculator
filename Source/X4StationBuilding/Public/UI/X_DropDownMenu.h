#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Service/X_Types.h"
#include "X_DropDownMenu.generated.h"


class UTextBlock;
class UX_StationsList;
class UButton;
class UMenuAnchor;
class UXDA_Stations;

UCLASS()
class X4STATIONBUILDING_API UX_DropDownMenu : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OpenMenu();

	UFUNCTION()
	UUserWidget* OnMenuOpen();

public:

	FTextDelegate OnStationSelected;

	UPROPERTY(EditAnywhere)
	UXDA_Stations* StationsDA;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UMenuAnchor* MenuAnchor;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* OpenButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* ButtonTextBlock;

	UPROPERTY(EditAnywhere)
	FText ButtonText;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UX_StationsList> ListClass;

	UPROPERTY()
	UX_StationsList* List;

	FText CurrentSelectedStation = FText::FromString("None");
};
