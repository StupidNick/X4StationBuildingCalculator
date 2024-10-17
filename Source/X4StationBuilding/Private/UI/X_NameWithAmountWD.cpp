#include "X_NameWithAmountWD.h"
#include "Components/TextBlock.h"



void UX_NameWithAmountWD::SetInfo(FName InNameObject, int32 InAmountObjects)
{
	if (!NameTextBlock || !AmountTextBlock) return;

	NameTextBlock->SetText(FText::FromName(InNameObject));
	AmountTextBlock->SetText(FText::AsNumber(InAmountObjects));
}
