#include "XDA_Stations.h"



FStationInfo UXDA_Stations::FindStationByName(FName InName)
{
	FStationInfo Result;
	Result.StationName = "None";
	Result.ManufacturedProduct = FObjectInfo();
	if (Stations.IsEmpty()) return Result;
	
	for (auto Element : Stations)
	{
		if (Element.StationName == InName)
		{
			return Element;
		}
	}
	return Result;
}

FStationInfo UXDA_Stations::FindStationByManufacturedProduct(FName InName)
{
	FStationInfo Result;
	Result.StationName = "None";
	Result.ManufacturedProduct = FObjectInfo();
	if (Stations.IsEmpty()) return Result;

	for (auto Element : Stations)
	{
		if (Element.ManufacturedProduct.Name == InName)
		{
			return Element;
		}
	}
	return Result;
}
