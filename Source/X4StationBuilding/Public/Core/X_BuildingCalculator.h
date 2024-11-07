#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XDA_Stations.h"
#include "X_BuildingCalculator.generated.h"


class UX_ObjectsDA;
class AX_HUD;
class UXDA_Stations;

UCLASS()
class X4STATIONBUILDING_API AX_BuildingCalculator : public AActor
{
	GENERATED_BODY()
	
public:	
	void AddStationsToList(const FText& InName, const int32 InNums);
	void ChangeStationsCountInList(const FText& InName, const int32 InOldNums, const int32 InNewNums);
	void RemoveStationsFromList(const FText& InName, const int32 InNums);

	void FillStationsList(TArray<FObjectInfo>& InStationsList, FResult& OutResult);
	void CalculateStationsAndProductsForSelectedStations();
	void ClearSelectedStations();

protected:
	virtual void BeginPlay() override;

private:
	//Main calculation functions
	void CalculateStationsOneLevelDown(const FText& InTargetStationName, int32 InTargetStationsNumber, bool bInRecursive, FResult& Result);
	
	void AddNecessaryProductToResult(const FObjectInfo& InConsumedProduct, int32 InTargetStationsNumber, FResult& Result);
	void AddNecessaryStationToResult(const FStationData& InManufacturedStation, int32 InProductsNumbers, FResult& Result);
	int32 CalculateNeededNumbersOfStations(int32 NeededProductsNumbers, const FStationData& ManufacturedStation) const;
	void CalculateResultProductsByStations(const TArray<FObjectInfo>& InStations, FResult& Result);
	void CalculateResultProductsAndStations(FResult& Result);

	//Workforce calculation functions
	void CalculateCommonWorkforce(FResult& Result);
	void RecalculateProductsWithWorkforce(FResult& Result) const;
	int32 GetProductNumbersByWorkforce(const FStationData& InStationInfo, const FResult& Result) const;

	//Production credits calculation functions
	void CalculateTotalMoneyPerHour(FResult& Result) const;
	int32 CalculateConsumedProductsCost(const FObjectInfo& CurrentConsumedProduct, FResult& InResult) const;
	int32 CalculateManufacturedProductsCost(const FObjectInfo& CurrentManufacturedProduct, FResult& InResult) const;

	//Station building cost calculation
	void CalculateStationBuildingCost(FResult& Result);

	//Service functions
	bool CheckLimitStations(const FObjectInfo& InSelectedStation, const int32 InNums);
	bool CheckLimitStations(const int32 InNums);
	bool FindStationInSelected(const FText& InName, const int32 InNums, FObjectInfo*& OutSelectedStation);
	bool FindStationInSelectedByName(const FText& InName, FObjectInfo*& OutSelectedStation);

public:

	FTextDelegate OnErrorDelegate;
	FSimpleDelegate OnSelectedStationsListCleared;
	FResultDelegate OnResultCalculated;

	UPROPERTY(EditAnywhere)
	UXDA_Stations* StationsDA;
	UPROPERTY(EditAnywhere)
	UX_ObjectsDA* ObjectsDA;

	UPROPERTY(EditAnywhere)
	int32 MaxNumsOfStations = 200;

	UPROPERTY()
	AX_HUD* HUD = nullptr;

private:
	TArray<FObjectInfo> SelectedStations;

	bool bBaseResourcesProvideByMiners;
	bool bResourcesProvideByOtherStations;
};
