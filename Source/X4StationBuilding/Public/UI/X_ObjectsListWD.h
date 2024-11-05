#pragma once

#include "CoreMinimal.h"
#include "X_Types.h"
#include "Blueprint/UserWidget.h"
#include "X_ObjectsListWD.generated.h"


class UX_NameWithAmountWD;
class UX_ObjectsDA;
class UVerticalBox;

UCLASS()
class X4STATIONBUILDING_API UX_ObjectsListWD : public UUserWidget
{
	GENERATED_BODY()

public:

	void CreateList(const TArray<FStationManufacturedInfo> InManufacturedStations, const TArray<FStationManufacturedInfo> InConsumedStations);
	void CreateList(const TArray<FStationWorkforceInfo> InManufacturedStations, const TArray<FStationWorkforceInfo> InConsumedStations);
	void CreateListForCosts(const TArray<FStationManufacturedInfo> InManufacturedStations);

private:
	void CreateNewLine(const TArray<FStationManufacturedInfo> InStations, bool bIsPositive);
	void CreateNewLine(const TArray<FStationWorkforceInfo> InStations, bool bIsPositive);
	void CreateNewLineForCosts(const TArray<FStationManufacturedInfo> InProducts, bool bIsPositive);

public:

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UX_NameWithAmountWD> NameWithAmountClass;

	UPROPERTY()
	TArray<UX_NameWithAmountWD*> Lines;
};
