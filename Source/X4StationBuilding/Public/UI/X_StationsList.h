#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Service/X_Types.h"
#include "X_StationsList.generated.h"


class UXDA_Stations;
class UX_ButtonWithText;
class UHorizontalBox;
class UVerticalBox;
class UScrollBox;

UCLASS()
class X4STATIONBUILDING_API UX_StationsList : public UUserWidget
{
	GENERATED_BODY()

public:

	void CreateList(UXDA_Stations* InStationsDA);

private:
	void CreateCategory(UXDA_Stations* InStationsDA, const int32 InCurrentCategoryID) const;

public:

	FTextDelegate OnStationSelected;
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UVerticalBox* VerticalBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UX_ButtonWithText> ButtonClass;

	UPROPERTY()
	TArray<UX_ButtonWithText*> Buttons;
};
