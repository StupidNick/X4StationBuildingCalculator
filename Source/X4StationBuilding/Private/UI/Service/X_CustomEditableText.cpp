#include "X_CustomEditableText.h"

#include <cwctype>


void UX_CustomEditableText::InitializeWidget()
{
	OnTextChanged.AddDynamic(this, &UX_CustomEditableText::OnTextChangedCallback);
	SetText(FText::FromString("1"));
}

void UX_CustomEditableText::OnTextChangedCallback(const FText& InText)
{
	const FString CurrentText = InText.ToString();
	if (CurrentText.Len() <= 0) return;
	
	FString ResultString;
	const int32 SymbolsCount = FMath::Min(MaxSymbolCount, CurrentText.Len()); 
	for (int i = 0; i < SymbolsCount; i++)
	{
		if (std::iswdigit(CurrentText[i]))
		{
			ResultString.AppendChar(CurrentText[i]);
		}
	}
	SetText(FText::FromString(ResultString));

	if (ResultString.Len() <= 0)
	{
		SetText(FText::FromString("1"));
		OnEditedTextChangedEvent.ExecuteIfBound(1);
		return;
	}
	OnEditedTextChangedEvent.ExecuteIfBound(FCString::Atoi(*ResultString));
}
