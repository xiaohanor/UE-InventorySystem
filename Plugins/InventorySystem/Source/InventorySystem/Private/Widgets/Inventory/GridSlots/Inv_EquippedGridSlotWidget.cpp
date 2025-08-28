// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlotWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "InventoryManagement/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_FragmentTags.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItemWidget.h"
#include "Widgets/Inventory/SlottedItem/Inv_EquippedSlottedItemWidget.h"

void UInv_EquippedGridSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!IsAvailable()) return;
	UInv_HoverItemWidget* HoverItem = UInv_InventoryStatics::GetHoverItem(GetOwningPlayer());
	if (!IsValid(HoverItem)) return;

	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag))
	{
		SetOccupiedTexture();
		Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Collapsed);
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
		Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Visible);
	}
}

FReply UInv_EquippedGridSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	EquippedGridSlotClicked.Broadcast(this, EquipmentTypeTag);
	return FReply::Handled();
}

UInv_EquippedSlottedItemWidget* UInv_EquippedGridSlotWidget::OnItemEquipped(UInv_InventoryItem* Item,
	const FGameplayTag& EquipmentTag, float TileSize)
{
	// 检查装备类型标签
	if (!EquipmentTag.MatchesTag(EquipmentTypeTag)) return nullptr;
	
	// 获取网格尺寸
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(Item, FragmentTags::GridFragment);
	if (!GridFragment) return nullptr;
	const FIntPoint GridDimensions = GridFragment->GetGridSize();
	
	// 计算已装备槽位物品的绘制尺寸
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;
	const FVector2D DrawSize = GridDimensions * IconTileWidth;
	
	// 创建已装备槽位物品控件
	EquippedSlottedItemWidget = CreateWidget<UInv_EquippedSlottedItemWidget>(GetOwningPlayer(), EquippedSlottedItemWidgetClass);
	
	// 设置槽位物品的库存物品
	EquippedSlottedItemWidget->SetInventoryItem(Item);
	
	// 设置槽位物品的装备类型标签
	EquippedSlottedItemWidget->SetEquipmentTypeTag(EquipmentTag);
	
	// 隐藏槽位物品上的堆栈计数控件
	EquippedSlottedItemWidget->UpdateStackCount(0);
	
	// 在此类别（EquippedGridSlotWidget）上设置库存物品
	SetInventoryItem(Item);
	
	// 在已装备槽位物品上设置图像画刷
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(Item, FragmentTags::IconFragment);
	if (!ImageFragment) return nullptr;

	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = DrawSize;
	
	EquippedSlottedItemWidget->SetImageBrush(Brush);
	
	// 将槽位物品作为子项添加到此控件的叠加层
	Overlay_Root->AddChildToOverlay(EquippedSlottedItemWidget);
	const FGeometry OverlayGeometry = Overlay_Root->GetCachedGeometry();
	const auto OverlaySize = OverlayGeometry.Size;

	const float LeftPadding = OverlaySize.X / 2.f - DrawSize.X / 2.f;
	const float TopPadding = OverlaySize.Y / 2.f - DrawSize.Y / 2.f;

	UOverlaySlot* OverlaySlot = UWidgetLayoutLibrary::SlotAsOverlaySlot(EquippedSlottedItemWidget);
	OverlaySlot->SetPadding(FMargin(LeftPadding, TopPadding));
	
	// 返回已装备槽位物品控件
	return EquippedSlottedItemWidget;
}
