#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Service/X_Types.h"
#include "X_ObjectsDA.generated.h"



UCLASS()
class X4STATIONBUILDING_API UX_ObjectsDA : public UDataAsset
{
	GENERATED_BODY()

public:

	bool FindObjectByName(const FText& InName, FProductInfo& OutObject) // TODO ask for this
	{
		if (Objects.IsEmpty()) return false;
		
		for (auto& Object : Objects)
		{
			if (Object.Name.ToString() == InName.ToString())
			{
				OutObject = Object;
				return true;
			}
		}
		return false;
	}

public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FProductInfo> Objects;
};
