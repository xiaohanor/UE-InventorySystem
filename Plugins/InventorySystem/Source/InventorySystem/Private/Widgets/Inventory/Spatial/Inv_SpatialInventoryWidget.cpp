// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/Inv_SpatialInventoryWidget.h"

#include "InventorySystem.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetSwitcher.h"
#include "InventoryManagement/Inv_InventoryStatics.h"
#include "InventoryManagement/Component/Inv_InventoryComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlotWidget.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItemWidget.h"
#include "Widgets/Inventory/SlottedItem/Inv_EquippedSlottedItemWidget.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGridWidget.h"
#include "Widgets/ItemDescription/Inv_ItemDescription.h"

void UInv_SpatialInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// NativeOnInitialized 只会初始化一次的逻辑，比如事件绑定
	Button_Equippables->OnClicked.AddDynamic(this, &ThisClass::ShowEquippables);
	Button_Consumables->OnClicked.AddDynamic(this, &ThisClass::ShowConsumables);
	Button_Craftables->OnClicked.AddDynamic(this, &ThisClass::ShowCraftables);

	Grid_Equippables->SetOwningCanvas(CanvasPanel);
	Grid_Consumables->SetOwningCanvas(CanvasPanel);
	Grid_Craftables->SetOwningCanvas(CanvasPanel);

	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		UInv_EquippedGridSlotWidget* EquippedGridSlot = Cast<UInv_EquippedGridSlotWidget>(Widget);
		if (IsValid(EquippedGridSlot))
		{
			EquippedGridSlots.Add(EquippedGridSlot);
			EquippedGridSlot->EquippedGridSlotClicked.AddDynamic(this, &ThisClass::EquippedGridSlotClicked);
		}
	});

	ShowEquippables();
}

void UInv_SpatialInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(ItemDescription)) return;
	SetItemDescriptionSizeAndPosition(GetItemDescription(), CanvasPanel);
}

FReply UInv_SpatialInventoryWidget::NativeOnMouseButtonDown(const FGeometry& MyGeometry,
                                                            const FPointerEvent& MouseEvent)
{
	ActiveGrid->DropItem();
	return FReply::Handled();
}

FInv_SlotAvailabilityResult UInv_SpatialInventoryWidget::HasRoomForItem(UInv_ItemComponent* ItemComponent) const
{
	switch (UInv_InventoryStatics::GetItemCategoryFromItemComp(ItemComponent))
	{
		case EInv_ItemCategory::Equippable:
			return Grid_Equippables->HasRoomForItem(ItemComponent);
		case EInv_ItemCategory::Consumable:
			return Grid_Consumables->HasRoomForItem(ItemComponent);
		case EInv_ItemCategory::Craftable:
			return Grid_Craftables->HasRoomForItem(ItemComponent);
		default:
			UE_LOG(LogInventory, Error, TEXT("ItemComponent doesn't have a valid Item Category."))
			return FInv_SlotAvailabilityResult();
	}
}

void UInv_SpatialInventoryWidget::OnItemHovered(UInv_InventoryItem* Item)
{
	// 延迟显示描述UI
	const auto& Manifest = Item->GetItemManifest();
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);

	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, &Manifest]()
	{
		Manifest.AssimilateInventoryFragments(GetItemDescription());
		GetItemDescription()->SetVisibility(ESlateVisibility::HitTestInvisible);
	});

	GetOwningPlayer()->GetWorldTimerManager().SetTimer(DescriptionTimer, TimerDelegate, DescriptionTimerDelay, false);
}

void UInv_SpatialInventoryWidget::OnItemUnHovered()
{
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
}

bool UInv_SpatialInventoryWidget::HasHoverItem() const
{
	if (Grid_Equippables->HasHoverItem()) return true;
	if (Grid_Consumables->HasHoverItem()) return true;
	if (Grid_Craftables->HasHoverItem()) return true;
	return false;
}

UInv_HoverItemWidget* UInv_SpatialInventoryWidget::GetHoverItem() const
{
	if (!ActiveGrid.IsValid()) return nullptr;
	return ActiveGrid->GetHoverItem();
}

bool UInv_SpatialInventoryWidget::CanEquipHoverItem(UInv_EquippedGridSlotWidget* EquippedGridSlot,
	const FGameplayTag& EquipmentTypeTag) const
{
	if (!IsValid(EquippedGridSlot) || EquippedGridSlot->GetInventoryItem().IsValid()) return false;

	UInv_HoverItemWidget* HoverItem = GetHoverItem();
	if (!IsValid(HoverItem)) return false;

	UInv_InventoryItem* HeldItem = HoverItem->GetInventoryItem();

	return HasHoverItem() && IsValid(HeldItem) &&
		!HoverItem->IsStackable() &&
			HeldItem->GetItemManifest().GetItemCategory() == EInv_ItemCategory::Equippable &&
				HeldItem->GetItemManifest().GetItemType().MatchesTag(EquipmentTypeTag);
}

float UInv_SpatialInventoryWidget::GetTileSize() const
{
	return Grid_Equippables->GetTileSize();
}

UInv_ItemDescription* UInv_SpatialInventoryWidget::GetItemDescription()
{
	if (!IsValid(ItemDescription))
	{
		ItemDescription = CreateWidget<UInv_ItemDescription>(GetOwningPlayer(), ItemDescriptionClass);
		CanvasPanel->AddChild(ItemDescription);
	}
	return ItemDescription;
	
}

void UInv_SpatialInventoryWidget::ShowEquippables()
{
	SetActiveGrid(Grid_Equippables, Button_Equippables);
}

void UInv_SpatialInventoryWidget::ShowConsumables()
{
	SetActiveGrid(Grid_Consumables, Button_Consumables);
}

void UInv_SpatialInventoryWidget::ShowCraftables()
{
	SetActiveGrid(Grid_Craftables, Button_Craftables);
}

void UInv_SpatialInventoryWidget::EquippedGridSlotClicked(UInv_EquippedGridSlotWidget* EquippedGridSlot,
	const FGameplayTag& EquipmentTypeTag)
{
	if (!CanEquipHoverItem(EquippedGridSlot, EquipmentTypeTag)) return;

	UInv_HoverItemWidget* HoverItem = GetHoverItem();
	// 创建一个已装备的槽位物品并将其添加到已装备的网格槽位（调用 EquippedGridSlot->OnItemEquipped()）
	const float TileSize = UInv_InventoryStatics::GetInventoryWidget(GetOwningPlayer())->GetTileSize();
	UInv_EquippedSlottedItemWidget* EquippedSlottedItem = EquippedGridSlot->OnItemEquipped(
		HoverItem->GetInventoryItem(),
		EquipmentTypeTag,
		TileSize
	);
	EquippedSlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this, &ThisClass::EquippedSlottedItemClicked);

	// 清除悬停物品
	Grid_Equippables->ClearHoverItem();
	
	// 通知服务器我们已经装备了一个物品（可能也卸下了一个物品）
	UInv_InventoryComponent* InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	check(IsValid(InventoryComponent)); 
	InventoryComponent->Server_EquipSlotClicked(HoverItem->GetInventoryItem(), nullptr);
}

void UInv_SpatialInventoryWidget::EquippedSlottedItemClicked(UInv_EquippedSlottedItemWidget* EquippedSlottedItem)
{
	// 移除物品描述
	UInv_InventoryStatics::ItemUnhovered(GetOwningPlayer());

	if (IsValid(GetHoverItem()) && GetHoverItem()->IsStackable()) return;
	// 获取要装备的物品
	UInv_InventoryItem* ItemToEquip = IsValid(GetHoverItem()) ? GetHoverItem()->GetInventoryItem() : nullptr;
	
	// 获取要卸下的物品
	UInv_InventoryItem* ItemToUnequip = EquippedSlottedItem->GetInventoryItem();
	
	// 获取持有此物品的已装备网格槽
	UInv_EquippedGridSlotWidget* EquippedGridSlot = FindSlotWithEquippedItem(ItemToUnequip);

	// 清除此物品的已装备网格槽（将其库存物品设置为空）
	ClearSlotOfItem(EquippedGridSlot);
	
	// 将之前装备的物品指定为悬停物品
	Grid_Equippables->AssignHoverItem(ItemToUnequip);
	
	// 从已装备网格槽中移除已装备槽位物品
	RemoveEquippedSlottedItem(EquippedSlottedItem);
	
	// 创建新的已装备槽位物品（用于我们 HoverItem 中持有的物品）
	MakeEquippedSlottedItem(EquippedSlottedItem, EquippedGridSlot, ItemToEquip);
	
	// 广播 OnItemEquipped/OnItemUnequipped 的委托（来自 IC）
	BroadcastSlotClickedDelegates(ItemToEquip, ItemToUnequip);
}

void UInv_SpatialInventoryWidget::DisableButton(UButton* Button)
{
	Button_Equippables->SetIsEnabled(true);
	Button_Consumables->SetIsEnabled(true);
	Button_Craftables->SetIsEnabled(true);
	Button->SetIsEnabled(false);
}

void UInv_SpatialInventoryWidget::SetActiveGrid(UInv_InventoryGridWidget* Grid, UButton* Button)
{
	if (ActiveGrid.IsValid())
	{
		ActiveGrid->HideCursor();
		ActiveGrid->OnHide();
	}
	ActiveGrid = Grid;
	if (ActiveGrid.IsValid()) ActiveGrid->ShowCursor();
	DisableButton(Button);
	Switcher->SetActiveWidget(Grid);
}

void UInv_SpatialInventoryWidget::SetItemDescriptionSizeAndPosition(UInv_ItemDescription* Description,
	UCanvasPanel* Canvas) const
{
	UCanvasPanelSlot* ItemDescriptionCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(Description);
	if (!IsValid(ItemDescriptionCPS)) return;

	const FVector2D ItemDescriptionSize = Description->GetBoxSize();
	ItemDescriptionCPS->SetSize(ItemDescriptionSize);

	FVector2D ClampedPosition = UInv_WidgetUtils::GetClampedWidgetPosition(
		UInv_WidgetUtils::GetWidgetSize(Canvas),
		ItemDescriptionSize,
		UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer()));

	ItemDescriptionCPS->SetPosition(ClampedPosition);
}

UInv_EquippedGridSlotWidget* UInv_SpatialInventoryWidget::FindSlotWithEquippedItem(
	UInv_InventoryItem* EquippedItem) const
{
	auto* FoundEquippedGridSlot  = EquippedGridSlots.FindByPredicate([EquippedItem](const UInv_EquippedGridSlotWidget* GridSlot)
	{
		return GridSlot->GetInventoryItem() == EquippedItem;
	});
	return FoundEquippedGridSlot  ? *FoundEquippedGridSlot  : nullptr;
}

void UInv_SpatialInventoryWidget::ClearSlotOfItem(UInv_EquippedGridSlotWidget* EquippedGridSlot)
{
	if (IsValid(EquippedGridSlot))
	{
		EquippedGridSlot->SetEquippedSlottedItem(nullptr);
		EquippedGridSlot->SetInventoryItem(nullptr);
	}
}

void UInv_SpatialInventoryWidget::RemoveEquippedSlottedItem(UInv_EquippedSlottedItemWidget* EquippedSlottedItem)
{
	// （从 OnEquippedSlottedItemClicked 解绑）
	// 从父级移除已装备槽位物品
	if (!IsValid(EquippedSlottedItem)) return;

	if (EquippedSlottedItem->OnEquippedSlottedItemClicked.IsAlreadyBound(this, &ThisClass::EquippedSlottedItemClicked))
	{
		EquippedSlottedItem->OnEquippedSlottedItemClicked.RemoveDynamic(this, &ThisClass::EquippedSlottedItemClicked);
	}
	EquippedSlottedItem->RemoveFromParent();
}

void UInv_SpatialInventoryWidget::MakeEquippedSlottedItem(UInv_EquippedSlottedItemWidget* EquippedSlottedItem,
	UInv_EquippedGridSlotWidget* EquippedGridSlot, UInv_InventoryItem* ItemToEquip)
{
	if (!IsValid(EquippedGridSlot)) return;

	UInv_EquippedSlottedItemWidget* SlottedItem = EquippedGridSlot->OnItemEquipped(
		ItemToEquip,
		EquippedSlottedItem->GetEquipmentTypeTag(),
		UInv_InventoryStatics::GetInventoryWidget(GetOwningPlayer())->GetTileSize());
	if (IsValid(SlottedItem)) SlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this, &ThisClass::EquippedSlottedItemClicked);

	EquippedGridSlot->SetEquippedSlottedItem(SlottedItem);
}

void UInv_SpatialInventoryWidget::BroadcastSlotClickedDelegates(UInv_InventoryItem* ItemToEquip,
	UInv_InventoryItem* ItemToUnequip) const
{
	UInv_InventoryComponent* InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	check(IsValid(InventoryComponent));
	InventoryComponent->Server_EquipSlotClicked(ItemToEquip, ItemToUnequip);
}
