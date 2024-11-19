#include "XDA_Stations.h"
#include "UObject/ObjectSaveContext.h"


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

#if WITH_EDITOR
void UXDA_Stations::PostSaveRoot(FObjectPostSaveRootContext ObjectSaveContext)
{
	Super::PostSaveRoot(ObjectSaveContext);

	SortStationsByCategory();
}

void UXDA_Stations::SortStationsByCategory()
{
	if (Stations.IsEmpty()) return;

	for (int i = 0; i < Stations.Num(); i++) 
	{
		for (int j = 0; j < Stations.Num() - 1; j++) 
		{
			if (Stations[j].SortID > Stations[j + 1].SortID) 
			{
				const FStationData b = Stations[j];
				Stations[j] = Stations[j + 1];
				Stations[j + 1] = b;
			}
		}
	}
}
#endif
