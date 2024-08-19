#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Service/X_Types.h"
#include "X_HUD.generated.h"


class AX_BuildingCalculator;
class UX_MainWidget;

UCLASS()
class X4STATIONBUILDING_API AX_HUD : public AHUD
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;

	void SetCalculator(AX_BuildingCalculator* InCalculator);

private:
	void CreateMainWidget();
	void DestroyMainWidget();

public:

	FTextDelegate OnErrorDelegate;
	FSimpleDelegate OnSelectedStationsListCleared;
	FArrayStationsDelegate OnSelectedStationAdded;
	FResultDelegate OnResultCalculated;

	FNameIntDelegate OnAddStationsButtonClicked;
	FSimpleDelegate OnCalculateButtonClickedEvent;
	FSimpleDelegate OnClearSelectedListButtonClickedEvent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UX_MainWidget> MainWidgetClass;
	UPROPERTY()
	UX_MainWidget* MainWidget = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> TestWidget;

private:

	UPROPERTY()
	AX_BuildingCalculator* Calculator;
};
