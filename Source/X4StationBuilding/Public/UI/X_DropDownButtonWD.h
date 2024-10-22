#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "X_DropDownButtonWD.generated.h"


struct FResult;
class UX_ObjectsListWD;
class UTextBlock;
class UButton;
class UMenuAnchor;

UCLASS()
class X4STATIONBUILDING_API UX_DropDownButton : public UUserWidget
{
	GENERATED_BODY()

public:

	void InitializeWidget(FName InName, FResult& InResult);

private:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OpenMenu();

	UFUNCTION()
	UUserWidget* OnMenuOpen();

public:

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UMenuAnchor* MenuAnchor;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UButton* OpenButton;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* NameTextBlock;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* AmountTextBlock;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UX_ObjectsListWD> ListClass;

private:

	UPROPERTY()
	UX_ObjectsListWD* List;
};
