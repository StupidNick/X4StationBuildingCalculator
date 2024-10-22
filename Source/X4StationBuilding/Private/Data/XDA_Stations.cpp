#include "XDA_Stations.h"



FStationData UXDA_Stations::FindStationByName(FName InName)
{
	FStationData Result;
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

FStationData UXDA_Stations::FindStationByManufacturedProduct(FName InName)
{
	FStationData Result;
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
