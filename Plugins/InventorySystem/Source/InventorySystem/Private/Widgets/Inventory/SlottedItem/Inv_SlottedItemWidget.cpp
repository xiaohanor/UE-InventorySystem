// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/SlottedItem/Inv_SlottedItemWidget.h"

#include "Components/Image.h"
#include "Items/Inv_InventoryItem.h"

void UInv_SlottedItemWidget::SetInventoryItem(UInv_InventoryItem* Item)
{
	InventoryItem = Item;
}

void UInv_SlottedItemWidget::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);
}
