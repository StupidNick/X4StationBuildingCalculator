#include "XDA_Stations.h"



bool UXDA_Stations::FindStationByName(FText InName, FStationData& Result)
{
	if (Stations.IsEmpty()) return false;
	
	for (auto Element : Stations)
	{
		if (Element.StationName.ToString() == InName.ToString())
		{
			Result = Element;
			return true;
		}
	}
	return false;
}

bool UXDA_Stations::FindStationByManufacturedProduct(FText InName, FStationData& Result)
{
	if (Stations.IsEmpty()) return false;

	for (auto Element : Stations)
	{
		if (Element.ManufacturedProduct.Name.ToString() == InName.ToString())
		{
			Result = Element;
			return true;
		}
	}
	return false;
}
