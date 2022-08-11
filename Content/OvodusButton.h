/* 
* Made by Ovodus (Dmitry Petrushin). ovodus@yandex.ru 
*/
#pragma once

#include "Components/Button.h"
#include "OvodusButton.generated.h"
/**
* Classic Slate Button with Advanced Hit implementation.
* Use SetAdvancedHitTexture to set unique button's geometry, determined by it's alpha channel.
* Use SetAdvancedHitAlpha to set an integer in range 0-255. If pixel's alpha is lower than this value, it will be treated as an empty space.
*/
class TESTPROJECT_API SOvodusButton : public SButton {
public:
	UTexture2D* AdvancedHitTexture;
	int AdvancedHitAlpha;

	SOvodusButton() : AdvancedHitTexture(NULL), AdvancedHitAlpha(0) {}

	void SetAdvancedHitTexture(UTexture2D* InTexture) { AdvancedHitTexture = InTexture; }
	void SetAdvancedHitAlpha(int InAlpha) { AdvancedHitAlpha = InAlpha; }

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override {
		if (!bIsHovered) return FReply::Unhandled();
		return SButton::OnMouseButtonDown(MyGeometry, MouseEvent);
	}
	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override {
		if (!bIsHovered) return FReply::Unhandled();
		return SButton::OnMouseButtonDoubleClick(InMyGeometry, InMouseEvent);
	}
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override {
		if (!bIsHovered) return FReply::Unhandled();
		return SButton::OnMouseButtonUp(MyGeometry, MouseEvent);
	}
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override {
		if (AdvancedHitTexture) return;
		return SButton::OnMouseEnter(MyGeometry, MouseEvent);
	}
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override {
		return SButton::OnMouseLeave(MouseEvent);
	}

	virtual FCursorReply OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const override;
	virtual TSharedPtr<IToolTip> GetToolTip() override { return (bIsHovered ? SWidget::GetToolTip() : NULL); }
};

/**
 * Classic UMG Button with Advanced Hit implementation.
 * Use SetAdvancedHitTexture to set unique button's geometry, determined by it's alpha channel.
 * Use SetAdvancedHitAlpha to set an integer in range 0-255. If pixel's alpha is lower than this value, it will be treated as an empty space.
 */
UCLASS()
class TESTPROJECT_API UOvodusButton : public UButton { GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AdvancedHitTest") UTexture2D* AdvancedHitTexture;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AdvancedHitTest", meta = (ClampMin = "0.0", ClampMax = "255.0", UIMin = "0.0", UIMax = "255.0")) int AdvancedHitAlpha;
	UFUNCTION(BlueprintCallable, Category = "AdvancedHitTest") void SetAdvancedHitTexture(UTexture2D* InTexture) { 
		AdvancedHitTexture = InTexture;
		if (MyButton.IsValid()) (static_cast<SOvodusButton*>(MyButton.Get()))->SetAdvancedHitTexture(AdvancedHitTexture);
	}
	UFUNCTION(BlueprintCallable, Category = "AdvancedHitTest") void SetAdvancedHitAlpha(int InAlpha) {
		AdvancedHitAlpha = InAlpha;
		if (MyButton.IsValid()) (static_cast<SOvodusButton*>(MyButton.Get()))->SetAdvancedHitAlpha(AdvancedHitAlpha);
	}

	UOvodusButton(const FObjectInitializer& ObjectInitializer) : AdvancedHitTexture(NULL), AdvancedHitAlpha(NULL), Super(ObjectInitializer) {}

	virtual void SynchronizeProperties() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
};
