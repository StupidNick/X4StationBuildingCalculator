#include "X_NameWithAmountWD.h"
#include "Components/TextBlock.h"



void UX_NameWithAmountWD::SetInfo(const FText& InNameObject, int32 InAmountObjects) const
{
	if (!NameTextBlock || !AmountTextBlock) return;

	NameTextBlock->SetText(InNameObject);
	AmountTextBlock->SetText(FText::AsNumber(InAmountObjects));
}
