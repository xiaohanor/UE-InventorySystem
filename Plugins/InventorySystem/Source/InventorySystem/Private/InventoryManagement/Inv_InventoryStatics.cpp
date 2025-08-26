// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Inv_InventoryStatics.h"

#include "InventoryManagement/Component/Inv_InventoryComponent.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBaseWidget.h"

UInv_InventoryComponent* UInv_InventoryStatics::GetInventoryComponent(const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return nullptr;

	UInv_InventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<UInv_InventoryComponent>();
	return InventoryComponent; 
}

EInv_ItemCategory UInv_InventoryStatics::GetItemCategoryFromItemComp(UInv_ItemComponent* ItemComp)
{
	if (!IsValid(ItemComp)) return EInv_ItemCategory::None;

	return ItemComp->GetItemManifest().GetItemCategory();
}

void UInv_InventoryStatics::ItemHovered(APlayerController* PC, UInv_InventoryItem* Item)
{
	UInv_InventoryComponent* InventoryComp = GetInventoryComponent(PC);
	if (!IsValid(InventoryComp)) return;

	UInv_InventoryBaseWidget* InventoryBase = InventoryComp->GetInventoryMenu();
	if (!IsValid(InventoryBase)) return;

	if (InventoryBase->HasHoverItem()) return;
	
	InventoryBase->OnItemHovered(Item);
}

void UInv_InventoryStatics::ItemUnhovered(APlayerController* PC)
{
	UInv_InventoryComponent* InventoryComp = GetInventoryComponent(PC);
	if (!IsValid(InventoryComp)) return;

	UInv_InventoryBaseWidget* InventoryBase = InventoryComp->GetInventoryMenu();
	if (!IsValid(InventoryBase)) return;

	InventoryBase->OnItemUnHovered();
}

UInv_HoverItemWidget* UInv_InventoryStatics::GetHoverItem(APlayerController* PC)
{
	UInv_InventoryComponent* InventoryComp = GetInventoryComponent(PC);
	if (!IsValid(InventoryComp)) return nullptr;
	
	UInv_InventoryBaseWidget* InventoryBase = InventoryComp->GetInventoryMenu();
	if (!IsValid(InventoryBase)) return nullptr;

	return InventoryBase->GetHoverItem();
}
