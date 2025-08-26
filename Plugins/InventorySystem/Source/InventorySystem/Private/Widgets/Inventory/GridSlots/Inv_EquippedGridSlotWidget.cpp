// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlotWidget.h"

#include "InventoryManagement/Inv_InventoryStatics.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItemWidget.h"

void UInv_EquippedGridSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!IsAvailable()) return;
	UInv_HoverItemWidget* HoverItem = UInv_InventoryStatics::GetHoverItem(GetOwningPlayer());
	if (!IsValid(HoverItem)) return;

	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag))
	{
		SetOccupiedTexture();
	}
}

void UInv_EquippedGridSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (!IsAvailable()) return;
	UInv_HoverItemWidget* HoverItem = UInv_InventoryStatics::GetHoverItem(GetOwningPlayer());
	if (!IsValid(HoverItem)) return;

	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag))
	{
		SetUnoccupiedTexture();
	}
}

FReply UInv_EquippedGridSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	EquippedGridSlotClicked.Broadcast(this, EquipmentTypeTag);
	return FReply::Handled();
}
