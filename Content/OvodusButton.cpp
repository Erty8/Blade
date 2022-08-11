/*
* Made by Ovodus (Dmitry Petrushin). ovodus@yandex.ru
*/

#include "TestProject.h"
#include "OvodusButton.h"

#pragma region >>> SOvodusButton <<<
FReply SOvodusButton::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	if (!AdvancedHitTexture) return SButton::OnMouseMove(MyGeometry, MouseEvent);
	bool WhatToReturn = true;
	FVector2D LocalPosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	LocalPosition.X = floor(LocalPosition.X);
	LocalPosition.Y = floor(LocalPosition.Y);
	LocalPosition /= MyGeometry.GetLocalSize();
	int ImageWidth = AdvancedHitTexture->PlatformData->SizeX;
	LocalPosition.X *= ImageWidth;
	LocalPosition.Y *= AdvancedHitTexture->PlatformData->SizeY;
	int BufferPosition = (LocalPosition.Y * ImageWidth) + LocalPosition.X;
	FColor* ImageData = static_cast<FColor*>((AdvancedHitTexture->PlatformData->Mips[0]).BulkData.Lock(LOCK_READ_ONLY));
	if (!ImageData) { WhatToReturn = false; }
	else { if (ImageData[BufferPosition].A <= AdvancedHitAlpha) WhatToReturn = false; }
	AdvancedHitTexture->PlatformData->Mips[0].BulkData.Unlock();
	if (WhatToReturn != bIsHovered) {
		bIsHovered = WhatToReturn;
		if (bIsHovered) SButton::OnMouseEnter(MyGeometry, MouseEvent);
		else SButton::OnMouseLeave(MouseEvent);
	}
	return SButton::OnMouseMove(MyGeometry, MouseEvent);
}
FCursorReply SOvodusButton::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const {
	if (!bIsHovered) return FCursorReply::Unhandled();
	TOptional<EMouseCursor::Type> TheCursor = Cursor.Get();
	return (TheCursor.IsSet()) ? FCursorReply::Cursor(TheCursor.GetValue()) : FCursorReply::Unhandled();
}
#pragma endregion

#pragma region >>> UOvodusButton <<<
void UOvodusButton::SynchronizeProperties() {
	Super::SynchronizeProperties();
	(static_cast<SOvodusButton*>(MyButton.Get()))->SetAdvancedHitTexture(AdvancedHitTexture);
	(static_cast<SOvodusButton*>(MyButton.Get()))->SetAdvancedHitAlpha(AdvancedHitAlpha);
}
TSharedRef<SWidget> UOvodusButton::RebuildWidget() {
	TSharedPtr<SOvodusButton> OvodusButton = SNew(SOvodusButton)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateHandleClicked))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressed))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleased))
		.OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
		.OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered)
		.ButtonStyle(&WidgetStyle)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.IsFocusable(IsFocusable)
		;

	OvodusButton->SetAdvancedHitTexture(AdvancedHitTexture);
	OvodusButton->SetAdvancedHitAlpha(AdvancedHitAlpha);

	MyButton = OvodusButton;

	if (GetChildrenCount() > 0) Cast<UButtonSlot>(GetContentSlot())->BuildSlot(MyButton.ToSharedRef());

	return MyButton.ToSharedRef();
}
#pragma endregion