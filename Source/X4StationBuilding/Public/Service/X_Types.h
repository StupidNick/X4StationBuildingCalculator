#pragma once

#include "CoreMinimal.h"
#include "X_Types.generated.h"



USTRUCT()
struct FNameAndNumbers
{
	GENERATED_BODY()

	FNameAndNumbers()
	{}
	FNameAndNumbers(FName InName, int32 InNumbers)
	{
		Name = InName;
		Numbers = InNumbers;
	}

	bool operator==(const FNameAndNumbers& left) const
	{
		return left.Name == Name && left.Numbers == Numbers;
	}

	UPROPERTY(EditAnywhere)
	FName Name;
	UPROPERTY(EditAnywhere)
	int32 Numbers;
};

USTRUCT()
struct FStationInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName StationName;

	UPROPERTY(EditDefaultsOnly)
	TArray<FNameAndNumbers> ConsumedProducts;

	UPROPERTY(EditDefaultsOnly)
	FNameAndNumbers ManufacturedProduct;
};

USTRUCT()
struct FResult
{
	GENERATED_BODY()

	TArray<FNameAndNumbers> ResultProductions;
	TArray<FNameAndNumbers> ResultStations;

	bool FindProductionsByName(FName InName, FNameAndNumbers*& OutProduction)
	{
		if (ResultProductions.IsEmpty()) return false;
		
		for (int i = 0; i < ResultProductions.Num(); i++)
		{
			if (ResultProductions[i].Name == InName)
			{
				OutProduction = &ResultProductions[i];
				return true;
			}
		}
		return false;
	}

	bool FindStationByName(FName InName, FNameAndNumbers*& OutStation)
	{
		if (ResultStations.IsEmpty()) return false;
		
		for (int i = 0; i < ResultStations.Num(); i++)
		{
			if (ResultStations[i].Name == InName)
			{
				OutStation = &ResultStations[i];
				return true;
			}
		}
		return false;
	}
};

DECLARE_DELEGATE_OneParam(FTextDelegate, FText)
DECLARE_DELEGATE_TwoParams(FNameIntDelegate, FName, int32);
DECLARE_DELEGATE_OneParam(FArrayStationsDelegate, TArray<FNameAndNumbers>);
DECLARE_DELEGATE_OneParam(FResultDelegate, FResult);