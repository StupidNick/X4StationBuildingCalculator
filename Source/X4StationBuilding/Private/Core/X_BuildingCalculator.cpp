#include "X_BuildingCalculator.h"
#include "X_HUD.h"
#include "X_Types.h"
#include "Kismet/GameplayStatics.h"



void AX_BuildingCalculator::AddStationsToList(const FText& InName, const int32 InNums) // TODO check
{
	if (!CheckLimitStations(InNums)) return;
	
	SelectedStations.Add(FObjectInfo(InName, InNums));
	CalculateStationsAndProductsForSelectedStations();
}

void AX_BuildingCalculator::ChangeStationsCountInList(const FText& InName, const int32 InOldNums, const int32 InNewNums) // TODO check
{
	FObjectInfo* TargetStationInfo;
	if (!FindStationInSelected(InName, InOldNums, TargetStationInfo) || !CheckLimitStations(*TargetStationInfo, InNewNums)) return;
	
	TargetStationInfo->Numbers = InNewNums;
	CalculateStationsAndProductsForSelectedStations();
}

void AX_BuildingCalculator::RemoveStationsFromList(const FText& InName, const int32 InNums) // TODO check
{
	FObjectInfo* TargetStationInfo;
	if (!FindStationInSelected(InName, InNums, TargetStationInfo)) return;

	SelectedStations.RemoveSingle(*TargetStationInfo);
	CalculateStationsAndProductsForSelectedStations();
}

void AX_BuildingCalculator::FillStationsList(TArray<FObjectInfo> InStationsList, FResult& OutResult)
{
	SelectedStations.Empty();
	for (const auto Station : InStationsList)
	{
		if (Station.Name.ToString() == "None" || Station.Numbers <= 0) continue;
		FObjectInfo* CurrentSelectedStation;
		if (FindStationInSelectedByName(Station.Name, CurrentSelectedStation))
		{
			CurrentSelectedStation->Numbers += Station.Numbers;
			continue;
		}
		SelectedStations.Add(Station);
	}

	if (SelectedStations.IsEmpty())
	{
		OnErrorDelegate.ExecuteIfBound(FText::FromString("You didn't select any station!"));
		return;
	}
	
	for (const auto Station : SelectedStations)
	{
		FObjectInfo* CheckedStation;
		if (OutResult.FindNecessaryStationByName(Station.Name, CheckedStation))
		{
			if (Station.Numbers <= CheckedStation->Numbers) continue;
			
			CalculateStationsOneLevelDown(Station.Name, Station.Numbers - CheckedStation->Numbers, true, OutResult);
			continue;
		}
		
		CalculateStationsOneLevelDown(Station.Name, Station.Numbers, true, OutResult);
	}
	CalculateResultProductsAndStations(OutResult);

	SelectedStations.Empty();
	for (auto Station : OutResult.ResultStations)
	{
		SelectedStations.Add(Station);
	}
	CalculateWorkforce(OutResult);
}

void AX_BuildingCalculator::CalculateStationsAndProductsForSelectedStations()
{
	if (!StationsDA || SelectedStations.IsEmpty())
	{
		OnErrorDelegate.ExecuteIfBound(FText::FromString("You didn't select any station!"));
		return;
	}

	FResult Result;
	for (const auto Station : SelectedStations)
	{
		FStationData TargetStationInfo;
		if (!StationsDA->FindStationByName(Station.Name, TargetStationInfo)) return;
	}
	for (const auto Station : SelectedStations)
	{
		CalculateStationsOneLevelDown(Station.Name, Station.Numbers, false, Result);
	}
	
	CalculateResultProductsByStations(SelectedStations, Result);
	CalculateWorkforce(Result);
	
	OnResultCalculated.ExecuteIfBound(Result);
}

void AX_BuildingCalculator::ClearSelectedStations()
{
	SelectedStations.Empty();
	OnSelectedStationsListCleared.ExecuteIfBound();
}

void AX_BuildingCalculator::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	HUD = Cast<AX_HUD>(PC->GetHUD());
	if (!HUD) return;

	HUD->AddStationEvent.BindUObject(this, &AX_BuildingCalculator::AddStationsToList);
	HUD->ChangeStationsCountEvent.BindUObject(this, &AX_BuildingCalculator::ChangeStationsCountInList);
	HUD->RemoveStationEvent.BindUObject(this, &AX_BuildingCalculator::RemoveStationsFromList);

	HUD->OnFillButtonClickedEvent.BindUObject(this, &AX_BuildingCalculator::FillStationsList);
	HUD->OnClearSelectedListButtonClickedEvent.BindUObject(this, &AX_BuildingCalculator::ClearSelectedStations);

	HUD->SetCalculator(this);
}

void AX_BuildingCalculator::CalculateStationsOneLevelDown(const FText& InTargetStationName,
                                                         int32 InTargetStationsNumber, bool bInRecursive, FResult& Result)
{
	if (!StationsDA || InTargetStationsNumber <= 0) return;

	FStationData TargetStationInfo;
	if (!StationsDA->FindStationByName(InTargetStationName, TargetStationInfo)) return;
	
	for (auto ConsumedProduct : TargetStationInfo.ConsumedProducts)
	{
		if (ConsumedProduct.Name.ToString() == "None") continue;

		// UE_LOG(LogTemp, Error, TEXT("For station %s x%i"), *TargetStationInfo.StationName.ToString(), InTargetStationsNumber);
		FObjectInfo* CurrentProductions;
		if (!Result.FindNecessaryProductsByName(ConsumedProduct.Name, CurrentProductions))
		{
			AddNecessaryProductToResult(ConsumedProduct, InTargetStationsNumber, Result);
			Result.FindNecessaryProductsByName(ConsumedProduct.Name, CurrentProductions);
			// UE_LOG(LogTemp, Warning, TEXT("Create %i necessary '%s' products == %i"),
				// InTargetStationsNumber * ConsumedProduct.Numbers, *CurrentProductions->Name.ToString(), CurrentProductions->Numbers);
		}
		else
		{
			CurrentProductions->Numbers += InTargetStationsNumber * ConsumedProduct.Numbers;
			// UE_LOG(LogTemp, Warning, TEXT("Add %i necessary '%s' products == %i"),
			// 	InTargetStationsNumber * ConsumedProduct.Numbers, *CurrentProductions->Name.ToString(), CurrentProductions->Numbers);
		}
		
		FStationData ProductManufacturedStation;// Station that manufactured current consumed product
		if (!StationsDA->FindStationByManufacturedProduct(ConsumedProduct.Name, ProductManufacturedStation)) continue;

		FObjectInfo* CurrentStations;
		if (!Result.FindNecessaryStationByName(ProductManufacturedStation.StationName, CurrentStations))
		{
			AddNecessaryStationToResult(ProductManufacturedStation, CurrentProductions->Numbers, Result);
			Result.FindNecessaryStationByName(ProductManufacturedStation.StationName, CurrentStations);
			if (bInRecursive)
			{
				CalculateStationsOneLevelDown(ProductManufacturedStation.StationName, CurrentStations->Numbers, true, Result);
			}
		}
		else
		{
			const int32 NewStationsNumber = CalculateNeededNumbersOfStations(CurrentProductions->Numbers, ProductManufacturedStation);
			if (CurrentStations->Numbers >= NewStationsNumber) continue;

			if (bInRecursive)
			{
				CalculateStationsOneLevelDown(ProductManufacturedStation.StationName, NewStationsNumber - CurrentStations->Numbers, true, Result);
			}
			CurrentStations->Numbers = NewStationsNumber;
		}
	}
}

void AX_BuildingCalculator::AddNecessaryProductToResult(const FObjectInfo& InConsumedProduct, int32 InTargetStationsNumber,
                                                        FResult& Result)
{
	FObjectInfo CurrentProductions;
	CurrentProductions.Name = InConsumedProduct.Name;
	CurrentProductions.Numbers = InConsumedProduct.Numbers * InTargetStationsNumber;
	Result.NecessaryProducts.Add(CurrentProductions);

	Result.AddUniqueToAllProducts(InConsumedProduct.Name);
}

void AX_BuildingCalculator::AddNecessaryStationToResult(const FStationData& InManufacturedStation, int32 InProductsNumbers,
	FResult& Result)
{
	FObjectInfo* ExistStation;
	if (Result.FindNecessaryStationByName(InManufacturedStation.StationName, ExistStation))
	{
		ExistStation->Numbers += CalculateNeededNumbersOfStations(InProductsNumbers, InManufacturedStation);
		return;
	}
	
	FObjectInfo CurrentStation;
	CurrentStation.Name = InManufacturedStation.StationName;
	CurrentStation.Numbers = CalculateNeededNumbersOfStations(InProductsNumbers, InManufacturedStation);
	Result.NecessaryStations.Add(CurrentStation);
}

int32 AX_BuildingCalculator::CalculateNeededNumbersOfStations(int32 NeededProductsNumbers,
	const FStationData& ManufacturedStation) const
{
	return FMath::CeilToInt32(static_cast<float>(NeededProductsNumbers) /
		static_cast<float>(ManufacturedStation.ManufacturedProduct.Numbers));
}

void AX_BuildingCalculator::CalculateResultProductsByStations(TArray<FObjectInfo> InStations, FResult& Result)
{
	for (const auto Station : InStations)
	{
		FStationData CurrentStation;
		if (!StationsDA->FindStationByName(Station.Name, CurrentStation) || CurrentStation.StationNotProduceAnything()) continue;

		FObjectInfo* ManufacturedProducts = nullptr;
		if (Result.FindResultProductsByName(CurrentStation.ManufacturedProduct.Name, ManufacturedProducts))
		{
			ManufacturedProducts->Numbers += Station.Numbers * CurrentStation.ManufacturedProduct.Numbers;
		}
		else
		{
			FObjectInfo Temp;
			Temp.Name = CurrentStation.ManufacturedProduct.Name;
			Temp.Numbers = Station.Numbers * CurrentStation.ManufacturedProduct.Numbers;
			Result.ResultProducts.Add(Temp);
		}

		FStationManufacturedInfo* ManufacturedProductInfo;
		if (Result.FindManufacturedProductByName(Station.Name, CurrentStation.ManufacturedProduct.Name, ManufacturedProductInfo))
		{
			ManufacturedProductInfo->ObjectsNumber += CurrentStation.ManufacturedProduct.Numbers * Station.Numbers;
			ManufacturedProductInfo->StationsNumber += Station.Numbers;
		}
		else
		{
			Result.StationsManufacturedProducts.Add(FStationManufacturedInfo(Station.Name,
												CurrentStation.ManufacturedProduct.Name,
												Station.Numbers,
												CurrentStation.ManufacturedProduct.Numbers * Station.Numbers));
		}
		
		Result.AddUniqueToAllProducts(CurrentStation.ManufacturedProduct.Name);

		if (CurrentStation.ConsumedProducts.IsEmpty()) continue;
		for (const auto& ConsumedProduct : CurrentStation.ConsumedProducts)
		{
			FStationManufacturedInfo* ConsumedProductInfo;
			if (Result.FindConsumedProductByName(Station.Name, ConsumedProduct.Name, ConsumedProductInfo))
			{
				ConsumedProductInfo->ObjectsNumber += ConsumedProduct.Numbers * Station.Numbers;
				ConsumedProductInfo->StationsNumber += Station.Numbers;
			}
			else
			{
				Result.StationsConsumedProducts.Add(FStationManufacturedInfo(Station.Name,
												ConsumedProduct.Name,
												Station.Numbers,
												ConsumedProduct.Numbers * Station.Numbers));
			}
		}
	}
}

void AX_BuildingCalculator::CalculateResultProductsAndStations(FResult& Result)
{
	Result.ResultStations.Append(SelectedStations);
	for (const auto Station : Result.NecessaryStations)
	{
		FObjectInfo* ManufacturedStation = nullptr;
		if (Result.FindResultStationByName(Station.Name, ManufacturedStation))
		{
			if (ManufacturedStation->Numbers >= Station.Numbers) continue;

			ManufacturedStation->Numbers = Station.Numbers;
		}
		else
		{
			Result.ResultStations.Add(FObjectInfo(Station.Name, Station.Numbers));
		}
	}

	CalculateResultProductsByStations(Result.ResultStations, Result);
}

void AX_BuildingCalculator::CalculateWorkforce(FResult& Result)
{
	for (const auto Station : SelectedStations)
	{
		FStationData CurrentStationData;
		if (!StationsDA->FindStationByName(Station.Name, CurrentStationData)) continue;

		FStationWorkforceInfo* CurrentStationWorkforce = nullptr;
		if (Result.FindWorkforceByStationName(Station.Name, CurrentStationWorkforce))
		{
			CurrentStationWorkforce->WorkforceNumber += CurrentStationData.WorkforceNumber * Station.Numbers;
			CurrentStationWorkforce->StationsNumber += Station.Numbers;
		}
		else
		{
			Result.WorkforceSummary.Add(FStationWorkforceInfo(Station.Name, Station.Numbers,
				CurrentStationData.WorkforceNumber * Station.Numbers));
		}
	}
}

bool AX_BuildingCalculator::CheckLimitStations(const FObjectInfo& InSelectedStation, const int32 InNums)
{
	bool bStationIsFound = false;
	int32 SelectedStationCounter = 0;
	for (const auto &Station : SelectedStations)
	{
		if (!bStationIsFound && Station.Name.ToString() == InSelectedStation.Name.ToString() && Station.Numbers == InSelectedStation.Numbers)
		{
			SelectedStationCounter += InNums;
			bStationIsFound= true;
			continue;
		}
		SelectedStationCounter += Station.Numbers;
	}
	if (SelectedStationCounter > MaxNumsOfStations)
	{
		OnErrorDelegate.ExecuteIfBound(FText::FromString("Sorry you want to build too much modules ):"));
		return false;
	}
	return true;
}

bool AX_BuildingCalculator::CheckLimitStations(const int32 InNums)
{
	int32 SelectedStationCounter = InNums;
	for (const auto &Station : SelectedStations)
	{
		SelectedStationCounter += Station.Numbers;
	}
	if (SelectedStationCounter > MaxNumsOfStations)
	{
		OnErrorDelegate.ExecuteIfBound(FText::FromString("Sorry you want to build too much modules ):"));
		return false;
	}
	return true;
}

bool AX_BuildingCalculator::FindStationInSelected(const FText& InName, const int32 InNums, FObjectInfo*& OutSelectedStation)
{
	// UE_LOG(LogTemp, Error, TEXT("Loocking for station %s x %i"), *InName.ToString(), InNums);
	for (auto& Station : SelectedStations)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Current station %s x %i"), *Station.Name.ToString(), Station.Numbers);
		if (Station.Name.ToString() == InName.ToString() && Station.Numbers == InNums)
		{
			OutSelectedStation = &Station;
			return true;
		}
	}
	return false;
}

bool AX_BuildingCalculator::FindStationInSelectedByName(const FText& InName, FObjectInfo*& OutSelectedStation)
{
	for (auto& Station : SelectedStations)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Current station %s x %i"), *Station.Name.ToString(), Station.Numbers);
		if (Station.Name.ToString() == InName.ToString())
		{
			OutSelectedStation = &Station;
			return true;
		}
	}
	return false;
}
