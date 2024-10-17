#pragma once

#include "CoreMinimal.h"
#include "X_Types.generated.h"



USTRUCT()
struct FObjectInfo
{
	GENERATED_BODY()

	FObjectInfo(){}
	FObjectInfo(FName InName, int32 InNumbers)
	{
		Name = InName;
		Numbers = InNumbers;
	}

	bool operator==(const FObjectInfo& left) const
	{
		return left.Name == Name && left.Numbers == Numbers && left.Cost == Cost;
	}

	UPROPERTY(EditAnywhere)
	FName Name;
	UPROPERTY(EditAnywhere)
	int32 Numbers;
	UPROPERTY(EditAnywhere)
	int32 Cost;
};

USTRUCT()
struct FObjectFromStation
{
	GENERATED_BODY()

	FObjectFromStation(const FName StationName, const FName ObjectName, int32 Numbers)
		: StationName(StationName),
		  ObjectName(ObjectName),
		  Numbers(Numbers)
	{
	}

	UPROPERTY(EditAnywhere)
	FName StationName;
	UPROPERTY(EditAnywhere)
	FName ObjectName;
	UPROPERTY(EditAnywhere)
	int32 Numbers;
};

USTRUCT()
struct FStationInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Common")
	FName StationName;

	UPROPERTY(EditDefaultsOnly, Category = "Manufacturing")
	TArray<FObjectInfo> ConsumedProducts;

	UPROPERTY(EditDefaultsOnly, Category = "Manufacturing")
	FObjectInfo ManufacturedProduct;

	UPROPERTY(EditDefaultsOnly, Category = "Manufacturing")
	int32 WorkforceNumber;

	UPROPERTY(EditDefaultsOnly, Category = "Manufacturing")
	float MaxEfficiency;

	UPROPERTY(EditDefaultsOnly, Category = "Building")
	TArray<FObjectInfo> ObjectsForBuilding;
};

USTRUCT()
struct FResult
{
	GENERATED_BODY()

	TArray<FObjectInfo> ResultProducts;
	TArray<FObjectInfo> NecessaryProducts;
	
	TArray<FObjectInfo> ResultStations;

	TArray<FObjectFromStation> ProductsByStations;

	bool FindNecessaryProductsByName(FName InName, FObjectInfo*& OutProduction)
	{
		if (NecessaryProducts.IsEmpty()) return false;
		
		for (int i = 0; i < NecessaryProducts.Num(); i++)
		{
			if (NecessaryProducts[i].Name == InName)
			{
				OutProduction = &NecessaryProducts[i];
				return true;
			}
		}
		return false;
	}

	bool FindResultProductsByName(FName InName, FObjectInfo*& OutProduction)
	{
		if (ResultProducts.IsEmpty()) return false;
		
		for (int i = 0; i < ResultProducts.Num(); i++)
		{
			if (ResultProducts[i].Name == InName)
			{
				OutProduction = &ResultProducts[i];
				return true;
			}
		}
		return false;
	}

	bool FindStationByName(FName InName, FObjectInfo*& OutStation)
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
DECLARE_DELEGATE_OneParam(FArrayStationsDelegate, TArray<FObjectInfo>);
DECLARE_DELEGATE_OneParam(FResultDelegate, FResult);