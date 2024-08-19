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
	FStationInfo FindStationByName(FName InName);
	UFUNCTION()
	FStationInfo FindStationByManufacturedProduct(FName InName);
	
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FStationInfo> Stations;
};
