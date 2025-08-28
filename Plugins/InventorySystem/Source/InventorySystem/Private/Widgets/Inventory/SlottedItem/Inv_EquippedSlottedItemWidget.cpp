// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/SlottedItem/Inv_EquippedSlottedItemWidget.h"

FReply UInv_EquippedSlottedItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	OnEquippedSlottedItemClicked.Broadcast(this);  
	return FReply::Handled();  
}
