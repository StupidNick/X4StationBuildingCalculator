#include "Core/X_BuildingCalculator.h"
#include "X_HUD.h"
#include "Service/X_Types.h"
#include "Kismet/GameplayStatics.h"



void AX_BuildingCalculator::AddStationsToList(FName InName, int32 InNums) 
{
	int32 SelectedStationCounter = InNums;
	for (const auto &Station : SelectedStations)
	{
		SelectedStationCounter += Station.Numbers;
	}
	if (SelectedStationCounter > MaxNumsOfStations)
	{
		OnErrorDelegate.ExecuteIfBound(FText::FromString("Sorry you want to build too much modules ):"));
		return;
	}

	for (auto &Station : SelectedStations)
	{
		if (InName == Station.Name)
		{
			Station.Numbers += InNums;
			if (Station.Numbers <= 0)
			{
				SelectedStations.RemoveSingle(Station);
			}
			OnSelectedStationAdded.ExecuteIfBound(SelectedStations);
			return;
		}
	}
	if (InNums <= 0) return;
	
	SelectedStations.Add(FObjectInfo(InName, InNums));
	OnSelectedStationAdded.ExecuteIfBound(SelectedStations);
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

	HUD->OnAddStationsButtonClicked.BindUObject(this, &AX_BuildingCalculator::AddStationsToList);
	HUD->OnCalculateButtonClickedEvent.BindUObject(this, &AX_BuildingCalculator::CalculateStationsAndProductsForSelectedStations);
	HUD->OnClearSelectedListButtonClickedEvent.BindUObject(this, &AX_BuildingCalculator::ClearSelectedStations);

	HUD->SetCalculator(this);
}

void AX_BuildingCalculator::CalculateStationsAndProducts(FName InTargetStationName,
                                                         int32 InTargetStationsNumber, FResult& Result)
{
	if (!StationsDA || InTargetStationsNumber <= 0) return;

	FStationData TargetStationInfo;
	if (!StationsDA->FindStationByName(InTargetStationName, TargetStationInfo)) return;
	UE_LOG(LogTemp, Warning, TEXT("Target station: %s x%i"), *TargetStationInfo.StationName.ToString(), InTargetStationsNumber);
	
	for (auto ConsumedProduct : TargetStationInfo.ConsumedProducts)
	{
		if (ConsumedProduct.Name == "None") continue;
		
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

		FObjectInfo* ManufacturedProducts;
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
