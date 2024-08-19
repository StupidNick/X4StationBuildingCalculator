#include "Core/X_BuildingCalculator.h"
#include "XDA_Stations.h"
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
	
	SelectedStations.Add(FNameAndNumbers(InName, InNums));
	OnSelectedStationAdded.ExecuteIfBound(SelectedStations);
}

void AX_BuildingCalculator::CalculateStationsAndProductsForSelectedStations()
{
	if (!StationsDA)
	{
		OnErrorDelegate.ExecuteIfBound(FText::FromString("You didn't select any station!"));
		return;
	}

	if (SelectedStations.IsEmpty())
	{
		OnErrorDelegate.ExecuteIfBound(FText::FromString("You didn't select any station!"));
		return;
	}

	FResult Result;
	for (const auto Station : SelectedStations) // Test more then one station 
	{
		CalculateStationsAndProducts(Station.Name, Station.Numbers, Result);
	}
	
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

	const FStationInfo TargetStationInfo = StationsDA->FindStationByName(InTargetStationName);
	if (TargetStationInfo.StationName == "None" || TargetStationInfo.ConsumedProducts.IsEmpty()) return;
	UE_LOG(LogTemp, Warning, TEXT("Target station: %s x%i"), *TargetStationInfo.StationName.ToString(), InTargetStationsNumber);
	
	for (auto ConsumedProduct : TargetStationInfo.ConsumedProducts)
	{
		if (ConsumedProduct.Name == "None") continue;
		
		FNameAndNumbers* CurrentProductions;
		if (!Result.FindProductionsByName(ConsumedProduct.Name, CurrentProductions))
		{
			AddProductToResult(ConsumedProduct, InTargetStationsNumber, Result);
			Result.FindProductionsByName(ConsumedProduct.Name, CurrentProductions);
		}
		else
		{
			CurrentProductions->Numbers += InTargetStationsNumber * ConsumedProduct.Numbers;
		}
		
		const FStationInfo ProductManufacturedStation = StationsDA->FindStationByManufacturedProduct(ConsumedProduct.Name);
		if (ProductManufacturedStation.StationName == "None") continue;
		UE_LOG(LogTemp, Log, TEXT("Consumed product: %s x%i"), *ConsumedProduct.Name.ToString(), ConsumedProduct.Numbers);

		FNameAndNumbers* CurrentStations;
		if (!Result.FindStationByName(ProductManufacturedStation.StationName, CurrentStations))
		{
			AddStationToResult(ProductManufacturedStation, CurrentProductions->Numbers, Result);
			Result.FindStationByName(ProductManufacturedStation.StationName, CurrentStations);
			CalculateStationsAndProducts(ProductManufacturedStation.StationName, CurrentStations->Numbers, Result);
		}
		else
		{
			const int32 NewStationsNumber = CalculateNeededNumbersOfStations(CurrentProductions->Numbers, ProductManufacturedStation);
			if (CurrentStations->Numbers == NewStationsNumber) continue;
			
			CalculateStationsAndProducts(ProductManufacturedStation.StationName, NewStationsNumber - CurrentStations->Numbers, Result);
			CurrentStations->Numbers = NewStationsNumber;
		}
		UE_LOG(LogTemp, Warning, TEXT("Manufactured station: %s x%i"), *CurrentStations->Name.ToString(), CurrentStations->Numbers);
	}
}

void AX_BuildingCalculator::AddProductToResult(FNameAndNumbers InConsumedProduct, int32 InTargetStationsNumber,
	FResult& Result)
{
	FNameAndNumbers CurrentProductions;
	CurrentProductions.Name = InConsumedProduct.Name;
	CurrentProductions.Numbers = InConsumedProduct.Numbers * InTargetStationsNumber;
	Result.ResultProductions.Add(CurrentProductions);
}

void AX_BuildingCalculator::AddStationToResult(FStationInfo InManufacturedStation, int32 InProductsNumbers, FResult& Result)
{
	FNameAndNumbers CurrentStation;
	CurrentStation.Name = InManufacturedStation.StationName;
	CurrentStation.Numbers = CalculateNeededNumbersOfStations(InProductsNumbers, InManufacturedStation);
	Result.ResultStations.Add(CurrentStation);
}

int32 AX_BuildingCalculator::CalculateNeededNumbersOfStations(int32 NeededProductsNumbers,
	FStationInfo ManufacturedStation)
{
	return FMath::CeilToInt32(static_cast<float>(NeededProductsNumbers) / static_cast<float>(ManufacturedStation.ManufacturedProduct.Numbers));
}
