#pragma once

#include "CoreMinimal.h"
#include "X_Types.generated.h"



USTRUCT()
struct FObjectInfo
{
	GENERATED_BODY()

	FObjectInfo(){}
	FObjectInfo(FText InName, int32 InNumbers)
	{
		Name = InName;
		Numbers = InNumbers;
	}

	bool operator==(const FObjectInfo& left) const
	{
		return left.Name.ToString() == Name.ToString() && left.Numbers == Numbers;
	}

	UPROPERTY(EditAnywhere)
	FText Name;
	UPROPERTY(EditAnywhere)
	int32 Numbers;
	UPROPERTY(EditAnywhere)
	int32 Cost;
};

USTRUCT()
struct FStationManufacturedInfo
{
	GENERATED_BODY()

	FStationManufacturedInfo(){}

	FStationManufacturedInfo(const FText InStationName, const FText InObjectName, int32 InStationNumbers, int32 InObjectNumbers)
		: StationName(InStationName), StationsNumber(InStationNumbers),
		  ObjectName(InObjectName),
		  ObjectsNumber(InObjectNumbers)
	{
	}

	UPROPERTY(EditAnywhere)
	FText StationName;
	UPROPERTY(EditAnywhere)
	int32 StationsNumber;
	UPROPERTY(EditAnywhere)
	FText ObjectName;
	UPROPERTY(EditAnywhere)
	int32 ObjectsNumber;
};

USTRUCT()
struct FStationData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Common")
	FText StationName;

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
	TArray<FObjectInfo> NecessaryStations;

	TArray<FStationManufacturedInfo> StationsConsumedProducts;
	TArray<FStationManufacturedInfo> StationsManufacturedProducts;

	TArray<FText> AllProducts;
	

	bool FindNecessaryProductsByName(const FText& InName, FObjectInfo*& OutProduction) // TODO maybe unification functions for find
	{
		if (NecessaryProducts.IsEmpty()) return false;
		
		for (int i = 0; i < NecessaryProducts.Num(); i++)
		{
			if (NecessaryProducts[i].Name.ToString() == InName.ToString())
			{
				OutProduction = &NecessaryProducts[i];
				return true;
			}
		}
		return false;
	}

	bool FindResultProductsByName(const FText& InName, FObjectInfo*& OutProduction)
	{
		if (ResultProducts.IsEmpty()) return false;
		
		for (int i = 0; i < ResultProducts.Num(); i++)
		{
			if (ResultProducts[i].Name.ToString() == InName.ToString())
			{
				OutProduction = &ResultProducts[i];
				return true;
			}
		}
		return false;
	}

	bool FindResultStationByName(const FText& InName, FObjectInfo*& OutStation)
	{
		if (ResultStations.IsEmpty()) return false;
		
		for (int i = 0; i < ResultStations.Num(); i++)
		{
			if (ResultStations[i].Name.ToString() == InName.ToString())
			{
				OutStation = &ResultStations[i];
				return true;
			}
		}
		return false;
	}

	bool FindNecessaryStationByName(const FText& InName, FObjectInfo*& OutStation)
	{
		if (NecessaryStations.IsEmpty()) return false;
		
		for (int i = 0; i < NecessaryStations.Num(); i++)
		{
			if (NecessaryStations[i].Name.ToString() == InName.ToString())
			{
				OutStation = &NecessaryStations[i];
				return true;
			}
		}
		return false;
	}

	TArray<FStationManufacturedInfo> FindAllManufacturedStationsByProductName(const FText& InProductName)
	{
		TArray<FStationManufacturedInfo> Result;
		
		for (auto Station : StationsManufacturedProducts)
		{
			if (Station.ObjectName.ToString() == InProductName.ToString())
			{
				Result.Add(Station);
			}
		}
		return Result;
	}

	TArray<FStationManufacturedInfo> FindAllConsumedStationsByProductName(const FText& InProductName)
	{
		TArray<FStationManufacturedInfo> Result;
		
		for (auto Station : StationsConsumedProducts)
		{
			if (Station.ObjectName.ToString() == InProductName.ToString())
			{
				Result.Add(Station);
			}
		}
		return Result;
	}

	bool FindConsumedProductByName(const FText& InStationName, const FText& InProductName, FStationManufacturedInfo*& OutStation)
	{
		if (StationsConsumedProducts.IsEmpty()) return false;
		
		for (int i = 0; i < StationsConsumedProducts.Num(); i++)
		{
			if (StationsConsumedProducts[i].StationName.ToString() == InStationName.ToString() &&
				StationsConsumedProducts[i].ObjectName.ToString() == InProductName.ToString())
			{
				OutStation = &StationsConsumedProducts[i];
				return true;
			}
		}
		return false;
	}

	bool FindManufacturedProductByName(const FText& InStationName, const FText& InProductName, FStationManufacturedInfo*& OutStation)
	{
		if (StationsManufacturedProducts.IsEmpty()) return false;
		
		for (int i = 0; i < StationsManufacturedProducts.Num(); i++)
		{
			if (StationsManufacturedProducts[i].StationName.ToString() == InStationName.ToString() &&
				StationsManufacturedProducts[i].ObjectName.ToString() == InProductName.ToString())
			{
				OutStation = &StationsManufacturedProducts[i];
				return true;
			}
		}
		return false;
	}

	void CheckAllProducts(const FText& InProductName)
	{
		for (auto Element : AllProducts)
		{
			if (Element.ToString() == InProductName.ToString()) return;
		}

		AllProducts.Add(InProductName);
	}

	bool IsEmpty() const
	{
		if (ResultProducts.IsEmpty() &&	NecessaryProducts.IsEmpty() &&
			ResultStations.IsEmpty() && NecessaryStations.IsEmpty() &&
			StationsConsumedProducts.IsEmpty() && StationsManufacturedProducts.IsEmpty() &&
			AllProducts.IsEmpty()) return true;
		return false;
	}

	void Empty()
	{
		ResultProducts.Empty();
		NecessaryProducts.Empty();
		ResultStations.Empty();
		NecessaryStations.Empty();
		StationsConsumedProducts.Empty();
		StationsManufacturedProducts.Empty();
		AllProducts.Empty();
	}
};

DECLARE_DELEGATE_OneParam(FTextDelegate, const FText&)
DECLARE_DELEGATE_OneParam(FInt32Delegate, int32)
DECLARE_DELEGATE_OneParam(FResultDelegate, FResult&)

DECLARE_DELEGATE_TwoParams(FTextInt32Delegate, const FText&, const int32)
DECLARE_DELEGATE_TwoParams(FFillStationsDelegate, TArray<FObjectInfo>, FResult&)

DECLARE_DELEGATE_ThreeParams(FChangeStationsCountDelegate, const FText&, const int32, const int32)
