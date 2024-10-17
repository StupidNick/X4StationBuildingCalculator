#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "X_ObjectsListWD.generated.h"


struct FResult;
struct FObjectInfo;
class UX_NameWithAmountWD;
class UX_ObjectsDA;
class UVerticalBox;

UCLASS()
class X4STATIONBUILDING_API UX_ObjectsListWD : public UUserWidget
{
	GENERATED_BODY()

public:

	void CreateList(TMap<FObjectInfo, bool> In);

public:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UVerticalBox* VerticalBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UX_NameWithAmountWD> NameWithAmountClass;
};
