#include "XDA_Stations.h"



bool UXDA_Stations::FindStationByName(const FText& InName, FStationData& Result)
{
	if (Stations.IsEmpty()) return false;
	
	for (auto Element : Stations)
	{
		UE_LOG(LogTemp, Error, TEXT("%s != %s"), *Element.StationName.ToString(), *InName.ToString());
		if (Element.StationName.ToString() == InName.ToString())
		{
			Result = Element;
			return true;
		}
	}
	return false;
}

bool UXDA_Stations::FindStationByManufacturedProduct(const FText& InName, FStationData& Result)
{
	if (Stations.IsEmpty()) return false;

	for (auto Element : Stations)
	{
		if (Element.DefaultManufacturedProduct.Name.ToString() == InName.ToString())
		{
			Result = Element;
			return true;
		}
	}
	return false;
}
