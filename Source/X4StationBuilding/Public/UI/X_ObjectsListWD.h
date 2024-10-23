#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "X_ObjectsListWD.generated.h"


struct FStationManufacturedInfo;
struct FResult;
struct FObjectInfo;
class UX_NameWithAmountWD;
class UX_ObjectsDA;
class UVerticalBox;

UCLASS()
class X4STATIONBUILDING_API UX_ObjectsListWD : public UUserWidget
{
	GENERATED_BODY()

public:

	void CreateList(const TArray<FStationManufacturedInfo>& InManufacturedStations, const TArray<FStationManufacturedInfo>& InConsumedStations);

private:
	void CreateNewLine(const TArray<FStationManufacturedInfo>& InStations, bool bIsPositive);

public:

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UX_NameWithAmountWD> NameWithAmountClass;

	UPROPERTY()
	TArray<UX_NameWithAmountWD*> Lines;
};
