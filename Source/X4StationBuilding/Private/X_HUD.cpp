#include "X_HUD.h"
#include "X_BuildingCalculator.h"
#include "UI/X_MainWidget.h"



void AX_HUD::BeginPlay()
{
	Super::BeginPlay();

	CreateMainWidget();
}

void AX_HUD::SetCalculator(AX_BuildingCalculator* InCalculator)
{
	if (!InCalculator) return;
	
	Calculator = InCalculator;
	Calculator->OnErrorDelegate.BindLambda([&](FText InText)
	{
		if (!MainWidget) return;

		MainWidget->PrintError(InText);
	});
	// Calculator->OnSelectedStationAdded.BindLambda([&](TArray<FObjectInfo> InStationsArray)
	// {
	// 	if (!MainWidget) return;
	// 	
	// 	MainWidget->SetStationsAndCount(InStationsArray);
	// });
	// Calculator->OnSelectedStationsListCleared.BindLambda([&]()
	// {
	// 	if (!MainWidget) return;
	//
	// 	MainWidget->ClearSelectedStationsList();
	// });
	Calculator->OnResultCalculated.BindLambda([&](FResult InResult)
	{
		if (!MainWidget) return;

		MainWidget->SetResult(InResult);
	});
}

void AX_HUD::CreateMainWidget()
{
	if (!MainWidgetClass || MainWidget) return;
	
	MainWidget = CreateWidget<UX_MainWidget>(GetWorld(), TestWidget);
	if (!MainWidget) return;
	
	MainWidget->AddToViewport();
	MainWidget->AddNewStationEvent.BindLambda([&](const FText& InName, const int32 InNums)
	{
		AddStationEvent.ExecuteIfBound(InName, InNums);
	});
	MainWidget->ChangeStationsCountEvent.BindLambda([&](const FText& InStationName, const int32 InOldCount, const int32 InNewCount)
	{
		ChangeStationsCountEvent.ExecuteIfBound(InStationName, InOldCount, InNewCount);
	});
	MainWidget->RemoveStationEvent.BindLambda([&](const FText& InStationName, const int32 InNums)
	{
		RemoveStationEvent.ExecuteIfBound(InStationName, InNums);
	});
	// MainWidget->OnCalculateButtonClickedEvent.BindLambda([&]()
	// {
	// 	OnCalculateButtonClickedEvent.ExecuteIfBound();
	// });
	MainWidget->OnClearSelectedListButtonClickedEvent.BindLambda([&]()
	{
		OnClearSelectedListButtonClickedEvent.ExecuteIfBound();
	});
}

void AX_HUD::DestroyMainWidget()
{
	if (!MainWidget) return;

	MainWidget->RemoveFromParent();
	MainWidget = nullptr;
}
