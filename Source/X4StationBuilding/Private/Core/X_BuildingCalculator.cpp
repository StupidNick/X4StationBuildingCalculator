#include "X_BuildingCalculator.h"
#include "X_HUD.h"
#include "X_Types.h"
#include "Kismet/GameplayStatics.h"



void AX_BuildingCalculator::AddStationsToList(const FText& InName, const int32 InNums) // TODO check
{
	UE_LOG(LogTemp, Warning, TEXT("Add station: %s x%i"), *InName.ToString(), InNums);
	if (!CheckLimitStations(InNums)) return;
	
	SelectedStations.Add(FObjectInfo(InName, InNums));
	CalculateStationsAndProductsForSelectedStations();
}

void AX_BuildingCalculator::ChangeStationsCountInList(const FText& InName, const int32 InOldNums, const int32 InNewNums) // TODO check
{
	UE_LOG(LogTemp, Warning, TEXT("Change '%s' station count: from %i to %i"), *InName.ToString(), InOldNums, InNewNums);
	FObjectInfo* TargetStationInfo;
	if (!FindStationInSelected(InName, InOldNums, TargetStationInfo) || !CheckLimitStations(*TargetStationInfo, InNewNums)) return;
	UE_LOG(LogTemp, Warning, TEXT("Station is found and have inside limit"));
	
	TargetStationInfo->Numbers = InNewNums;
	CalculateStationsAndProductsForSelectedStations();
}

void AX_BuildingCalculator::RemoveStationsFromList(const FText& InName, const int32 InNums) // TODO check
{
	FObjectInfo* TargetStationInfo;
	if (!FindStationInSelected(InName, InNums, TargetStationInfo)) return;

	SelectedStations.RemoveSingle(*TargetStationInfo);
	for (auto Element : SelectedStations)
	{
		UE_LOG(LogTemp, Warning, TEXT("Element: %s x%i"), *Element.Name.ToString(), Element.Numbers);
	}
	CalculateStationsAndProductsForSelectedStations();
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
		
		AddStationToResult(TargetStationInfo, Station.Numbers * TargetStationInfo.ManufacturedProduct.Numbers, Result);
	}
	for (const auto Station : SelectedStations) // Test more then one station 
	{
		CalculateStationsAndProducts(Station.Name, Station.Numbers, Result);
	}
	
	CalculateResultProducts(Result);
	
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
	
	HUD->OnClearSelectedListButtonClickedEvent.BindUObject(this, &AX_BuildingCalculator::ClearSelectedStations);

	HUD->SetCalculator(this);
}

void AX_BuildingCalculator::CalculateStationsAndProducts(const FText& InTargetStationName,
                                                         int32 InTargetStationsNumber, FResult& Result)
{
	if (!StationsDA || InTargetStationsNumber <= 0) return;

	FStationData TargetStationInfo;
	if (!StationsDA->FindStationByName(InTargetStationName, TargetStationInfo)) return;
	UE_LOG(LogTemp, Warning, TEXT("Target station: %s x%i"), *TargetStationInfo.StationName.ToString(), InTargetStationsNumber);
	
	for (auto ConsumedProduct : TargetStationInfo.ConsumedProducts)
	{
		if (ConsumedProduct.Name.ToString() == "None") continue;
		
		FObjectInfo* CurrentProductions;
		if (!Result.FindNecessaryProductsByName(ConsumedProduct.Name, CurrentProductions))
		{
			AddNecessaryProductToResult(ConsumedProduct, InTargetStationsNumber, Result);
			Result.FindNecessaryProductsByName(ConsumedProduct.Name, CurrentProductions);
		}
		else
		{
			CurrentProductions->Numbers += InTargetStationsNumber * ConsumedProduct.Numbers;
		}
		
		FStationData ProductManufacturedStation;// Station that manufactured current consumed product
		if (!StationsDA->FindStationByManufacturedProduct(ConsumedProduct.Name, ProductManufacturedStation)) continue;
		UE_LOG(LogTemp, Log, TEXT("Consumed product: %s x%i"), *ConsumedProduct.Name.ToString(), ConsumedProduct.Numbers);

		FObjectInfo* CurrentStations;
		if (!Result.FindStationByName(ProductManufacturedStation.StationName, CurrentStations))
		{
			AddStationToResult(ProductManufacturedStation, CurrentProductions->Numbers, Result);
			Result.FindStationByName(ProductManufacturedStation.StationName, CurrentStations);
			CalculateStationsAndProducts(ProductManufacturedStation.StationName, CurrentStations->Numbers, Result);
		}
		else
		{
			const int32 NewStationsNumber = CalculateNeededNumbersOfStations(CurrentProductions->Numbers, ProductManufacturedStation);
			if (CurrentStations->Numbers >= NewStationsNumber) continue;
			
			CalculateStationsAndProducts(ProductManufacturedStation.StationName, NewStationsNumber - CurrentStations->Numbers, Result);
			CurrentStations->Numbers = NewStationsNumber;
		}
		UE_LOG(LogTemp, Warning, TEXT("Manufactured station: %s x%i"), *CurrentStations->Name.ToString(), CurrentStations->Numbers);
	}
}

void AX_BuildingCalculator::AddNecessaryProductToResult(const FObjectInfo& InConsumedProduct, int32 InTargetStationsNumber,
	FResult& Result)
{
	FObjectInfo CurrentProductions;
	CurrentProductions.Name = InConsumedProduct.Name;
	CurrentProductions.Numbers = InConsumedProduct.Numbers * InTargetStationsNumber;
	Result.NecessaryProducts.Add(CurrentProductions);
}

void AX_BuildingCalculator::AddStationToResult(const FStationData& InManufacturedStation, int32 InProductsNumbers,
	FResult& Result)
{
	FObjectInfo* ExistStation;
	if (Result.FindStationByName(InManufacturedStation.StationName, ExistStation))
	{
		ExistStation->Numbers += CalculateNeededNumbersOfStations(InProductsNumbers, InManufacturedStation);
		return;
	}
	
	FObjectInfo CurrentStation;
	CurrentStation.Name = InManufacturedStation.StationName;
	CurrentStation.Numbers = CalculateNeededNumbersOfStations(InProductsNumbers, InManufacturedStation);
	Result.ResultStations.Add(CurrentStation);
}

int32 AX_BuildingCalculator::CalculateNeededNumbersOfStations(int32 NeededProductsNumbers,
	const FStationData& ManufacturedStation) const
{
	return FMath::CeilToInt32(static_cast<float>(NeededProductsNumbers) /
		static_cast<float>(ManufacturedStation.ManufacturedProduct.Numbers));
}

void AX_BuildingCalculator::CalculateResultProducts(FResult& Result)
{
	for (const auto Station : Result.ResultStations)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Result station: %s"), *Station.Name.ToString());
		FStationData CurrentStation;
		if (!StationsDA->FindStationByName(Station.Name, CurrentStation)) continue;

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

		Result.StationsManufacturedProducts.Add(FStationManufacturedInfo(Station.Name,
												CurrentStation.ManufacturedProduct.Name,
												Station.Numbers,
												CurrentStation.ManufacturedProduct.Numbers * Station.Numbers));

		if (CurrentStation.ConsumedProducts.IsEmpty()) continue;
		for (const auto& ConsumedProduct : CurrentStation.ConsumedProducts)
		{
			Result.StationsConsumedProducts.Add(FStationManufacturedInfo(Station.Name,
												ConsumedProduct.Name,
												Station.Numbers,
												ConsumedProduct.Numbers * Station.Numbers));// TODO check this
			// UE_LOG(LogTemp, Warning, TEXT("ConsumedProduct products: %s x%i"), *ConsumedProduct.Name.ToString(), ConsumedProduct.Numbers * Station.Numbers);
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
	for (auto& Station : SelectedStations)
	{
		UE_LOG(LogTemp, Warning, TEXT("Station '%s' with nums %i is equal to station '%s' with nums %i?"), *Station.Name.ToString(), Station.Numbers, *InName.ToString(), InNums);
		if (Station.Name.ToString() == InName.ToString() && Station.Numbers == InNums)
		{
			UE_LOG(LogTemp, Log, TEXT("Yes"));
			OutSelectedStation = &Station;
			return true;
		}
		UE_LOG(LogTemp, Error, TEXT("No"));
	}
	return false;
}
