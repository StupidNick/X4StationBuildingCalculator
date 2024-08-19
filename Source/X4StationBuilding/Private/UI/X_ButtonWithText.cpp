#include "UI/X_ButtonWithText.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"


void UX_ButtonWithText::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button->OnClicked.AddDynamic(this, &UX_ButtonWithText::OnButtonPressed);
}

void UX_ButtonWithText::OnButtonPressed()
{
	OnButtonPressedEvent.ExecuteIfBound(Text->GetText());
}

void UX_ButtonWithText::SetText(const FText InText) const
{
	if (!Text) return;

	Text->SetText(InText);
}

void UX_ButtonWithText::SetText(const FName InText) const
{
	SetText(FText::FromName(InText));
}
