#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Service/X_Types.h"
#include "XDA_Stations.generated.h"



UCLASS()
class X4STATIONBUILDING_API UXDA_Stations : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION()
	bool FindStationByName(const FText& InName, FStationData& Result);
	UFUNCTION()
	bool FindStationByManufacturedProduct(const FText& InName, FStationData& Result);
	
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FStationData> Stations;
};
