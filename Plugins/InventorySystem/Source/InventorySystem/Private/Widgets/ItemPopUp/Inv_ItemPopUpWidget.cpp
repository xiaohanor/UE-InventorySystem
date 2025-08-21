// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ItemPopUp/Inv_ItemPopUpWidget.h"

#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

void UInv_ItemPopUpWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Split->OnClicked.AddDynamic(this, &ThisClass::SplitButtonClicked);
	Button_Drop->OnClicked.AddDynamic(this, &ThisClass::DropButtonClicked);
	Button_Consume->OnClicked.AddDynamic(this, &ThisClass::ConsumeButtonClicked);
	Slider_Split->OnValueChanged.AddDynamic(this, &ThisClass::SliderValueChanged);
}

void UInv_ItemPopUpWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	RemoveFromParent();
}

int32 UInv_ItemPopUpWidget::GetSplitAmount() const
{
	return FMath::Floor(Slider_Split->GetValue());
}

void UInv_ItemPopUpWidget::CollapseSplitButton() const
{
	Button_Split->SetVisibility(ESlateVisibility::Collapsed);
	Slider_Split->SetVisibility(ESlateVisibility::Collapsed);
	Text_SplitAmount->SetVisibility(ESlateVisibility::Collapsed);
}

void UInv_ItemPopUpWidget::CollapseConsumeButton() const
{
	Button_Consume->SetVisibility(ESlateVisibility::Collapsed);
}

void UInv_ItemPopUpWidget::SetSliderParams(const float Max, const float Value) const
{
	Slider_Split->SetMinValue(1);
	Slider_Split->SetMaxValue(Max);
	Slider_Split->SetValue(Value);
	Text_SplitAmount->SetText(FText::AsNumber(FMath::Floor(Value)));
}

FVector2D UInv_ItemPopUpWidget::GetBoxSize() const
{
	return FVector2D(SizeBox_Root->GetWidthOverride(), SizeBox_Root->GetHeightOverride());
}

void UInv_ItemPopUpWidget::SplitButtonClicked()
{
	if (OnSplit.ExecuteIfBound(GetSplitAmount(), GridIndex))
	{
		RemoveFromParent();
	}
}

void UInv_ItemPopUpWidget::DropButtonClicked()
{
	if (OnDrop.ExecuteIfBound(GridIndex))
	{
		RemoveFromParent();
	}
}

void UInv_ItemPopUpWidget::ConsumeButtonClicked()
{
	if (OnConsume.ExecuteIfBound(GridIndex))
	{
		RemoveFromParent();
	}
}

void UInv_ItemPopUpWidget::SliderValueChanged(float Value)
{
	Text_SplitAmount->SetText(FText::AsNumber(FMath::Floor(Value)));
}
