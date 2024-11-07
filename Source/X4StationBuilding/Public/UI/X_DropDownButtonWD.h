#pragma once

#include "CoreMinimal.h"
#include "X_Types.h"
#include "Blueprint/UserWidget.h"
#include "X_DropDownButtonWD.generated.h"


class UVerticalBox;
struct FResult;
class UX_ObjectsListWD;
class UTextBlock;
class UButton;

UCLASS()
class X4STATIONBUILDING_API UX_DropDownButton : public UUserWidget
{
	GENERATED_BODY()

public:

	void InitializeWidgetAsProductsInfo(const FText& InName, FResult& InResult);
	void InitializeWidgetAsWorkforceInfo(const FResult& InResult);
	void InitializeWidgetAsResultCostsInfo(const TArray<FProductCostInfo>& InCostInfo, const int32 InTotalCost);
	void InitializeWidgetAsStationCostsInfo(const TArray<FStationBuildingInfo>& InCostInfo);

private:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OpenMenu();
	void SetTextColor(const FLinearColor& InColor) const;

public:

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* DetailsVerticalBox;

	UPROPERTY(meta = (BindWidget))
	UButton* OpenButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmountTextBlock;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UX_ObjectsListWD> ListClass;

	UPROPERTY(EditDefaultsOnly)
	FText WorkforceName;

	UPROPERTY(EditDefaultsOnly)
	FText ExpensesName;

	UPROPERTY(EditDefaultsOnly)
	FText ProductionName;

private:

	UPROPERTY()
	UX_ObjectsListWD* List;

	bool bIsOpen = false;
};
