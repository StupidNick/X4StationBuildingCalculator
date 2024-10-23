#include "XDA_Stations.h"



bool UXDA_Stations::FindStationByName(FName InName, FStationData& Result)
{
	if (Stations.IsEmpty()) return false;
	
	for (auto Element : Stations)
	{
		if (Element.StationName == InName)
		{
			Result = Element;
			return true;
		}
	}
	return false;
}

bool UXDA_Stations::FindStationByManufacturedProduct(FName InName, FStationData& Result)
{
	if (Stations.IsEmpty()) return false;

	for (auto Element : Stations)
	{
		if (Element.ManufacturedProduct.Name == InName)
		{
			Result = Element;
			return true;
		}
	}
	return false;
}
