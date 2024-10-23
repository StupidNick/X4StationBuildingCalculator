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
	bool FindStationByName(FName InName, FStationData& Result);
	UFUNCTION()
	bool FindStationByManufacturedProduct(FName InName, FStationData& Result);
	
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FStationData> Stations;
};
