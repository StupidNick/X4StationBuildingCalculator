#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XDA_Stations.h"
#include "Service/X_Types.h"
#include "X_BuildingCalculator.generated.h"


class UX_ObjectsDA;
class AX_HUD;
class UXDA_Stations;

UCLASS()
class X4STATIONBUILDING_API AX_BuildingCalculator : public AActor
{
	GENERATED_BODY()
	
public:	
	void AddStationsToList(FName InName, int32 InNums);
	void CalculateStationsAndProductsForSelectedStations();
	void ClearSelectedStations();

protected:
	virtual void BeginPlay() override;

private:
	void CalculateStationsAndProducts(FName InTargetStationName, int32 InTargetStationsNumber, FResult& Result);
	void AddNecessaryProductToResult(const FObjectInfo& InConsumedProduct, int32 InTargetStationsNumber, FResult& Result);
	void AddProductToResult(const FObjectInfo& InManufacturedProduct, int32 InTargetStationsNumber, FResult& Result);
	void AddStationToResult(const FStationData& InManufacturedStation, int32 InProductsNumbers, FResult& Result);
	int32 CalculateNeededNumbersOfStations(int32 NeededProductsNumbers, const FStationData& ManufacturedStation) const;
	void CalculateResultProducts(FResult& Result);

public:

	FTextDelegate OnErrorDelegate;
	FSimpleDelegate OnSelectedStationsListCleared;
	FArrayStationsDelegate OnSelectedStationAdded;
	FResultDelegate OnResultCalculated;

	UPROPERTY(EditAnywhere)
	UXDA_Stations* StationsDA;
	UPROPERTY(EditAnywhere)
	UX_ObjectsDA* ObjectsDA;

	UPROPERTY(EditAnywhere)
	int32 MaxNumsOfStations = 200;

	TArray<FObjectInfo> SelectedStations;

	UPROPERTY()
	AX_HUD* HUD = nullptr;
};
