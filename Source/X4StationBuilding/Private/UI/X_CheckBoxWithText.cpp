#include "X_CheckBoxWithText.h"

#include "Components/CheckBox.h"
#include "Components/TextBlock.h"


void UX_CheckBoxWithText::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!CheckBox) return;

	CheckBox->OnCheckStateChanged.AddDynamic(this, &UX_CheckBoxWithText::OnValueChanged);
}

void UX_CheckBoxWithText::OnValueChanged(bool InValue)
{
	OnValueChangedEvent.ExecuteIfBound(InValue);
}

void UX_CheckBoxWithText::SetText(const FText& InText) const
{
	if (!Text) return;

	Text->SetText(InText);
}
