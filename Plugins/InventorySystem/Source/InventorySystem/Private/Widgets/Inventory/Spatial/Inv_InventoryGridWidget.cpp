// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/Inv_InventoryGridWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagement/Inv_InventoryStatics.h"
#include "InventoryManagement/Component/Inv_InventoryComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Fragments/Inv_FragmentTags.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlotWidget.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItemWidget.h"
#include "Widgets/Inventory/SlottedItem/Inv_SlottedItemWidget.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"

void UInv_InventoryGridWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAdded.AddDynamic(this, &ThisClass::AddItem);
	InventoryComponent->OnStackChange.AddDynamic(this, &ThisClass::AddStacks);
}

FInv_SlotAvailabilityResult UInv_InventoryGridWidget::HasRoomForItem(const UInv_ItemComponent* ItemComponent)
{
	return HasRoomForItem(ItemComponent->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGridWidget::HasRoomForItem(const UInv_InventoryItem* Item)
{
	return HasRoomForItem(Item->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGridWidget::HasRoomForItem(const FInv_ItemManifest& Manifest)
{
	FInv_SlotAvailabilityResult Result;

	// 判断物品是否可堆叠
	const FInv_StackableFragment* StackableFragment = Manifest.GetTypeOfFragment<FInv_StackableFragment>();
	Result.bStackable = StackableFragment != nullptr;

	// 判断堆叠数量
	const int32 MaxStackSize = StackableFragment ? StackableFragment->GetMaxStackSize() : 1;
	int32 AmountToFill = StackableFragment ? StackableFragment->GetStackCount() : 1;

	TSet<int32> CheckedIndices;
	for (const auto& GridSlot : GridSlots)
	{
		// 没有需要填充的数量，跳出
		if (AmountToFill == 0) break;

		// 索引已经检查过了，跳过
		if (IsIndexClaimed(CheckedIndices, GridSlot->GetIndex())) continue;

		// 物品在网格边界内吗?
		if (!IsInGridBounds(GridSlot->GetIndex(), GetItemDimensions(Manifest))) continue;

		// 物品可以填充在该格子吗?
		TSet<int32> TentativelyClaimed;	// 暂时占领的格子
		if (!HasRoomAtIndex(GridSlot, GetItemDimensions(Manifest), CheckedIndices, TentativelyClaimed, Manifest.GetItemType(),MaxStackSize))
		{
			continue;
		}

		// 填充多少?
		const int32 AmountToFillInSlot = DetermineFillAmountForSlot(Result.bStackable, MaxStackSize, AmountToFill, GridSlot);
		if (AmountToFillInSlot == 0) continue;
		
		CheckedIndices.Append(TentativelyClaimed);
		
		// 更新剩余待填数量
		Result.TotalRoomToFill += AmountToFillInSlot;
		Result.SlotAvailabilities.Emplace(
			FInv_SlotAvailability{
				HasValidItem(GridSlot) ? GridSlot->GetUpperLeftIndex() : GridSlot->GetIndex(),
				Result.bStackable ? AmountToFillInSlot : 0,
				HasValidItem(GridSlot)
			}
		);

		AmountToFill -= AmountToFillInSlot;

		// 无法放置的剩余数量是多少
		Result.Remainder = AmountToFill;

		if (AmountToFill == 0) return Result;
	}
	
	return Result;
}

void UInv_InventoryGridWidget::AddItem(UInv_InventoryItem* Item)
{
	if (!MatchesCategory(Item)) return;

	FInv_SlotAvailabilityResult Result = HasRoomForItem(Item);
	AddItemToIndices(Result, Item);
}

void UInv_InventoryGridWidget::AddItemToIndices(const FInv_SlotAvailabilityResult& Result,UInv_InventoryItem* NewItem)
{
	for (const auto& Availability : Result.SlotAvailabilities)
	{
		AddItemAtIndex(NewItem, Availability.Index, Result.bStackable, Availability.AmountToFill);
		UpdateGridSlots(NewItem, Availability.Index, Result.bStackable, Availability.AmountToFill);
	}
}

void UInv_InventoryGridWidget::AddItemAtIndex(UInv_InventoryItem* Item, const int32 Index, const bool bStackable,
	const int32 StackAmount)
{
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(Item, FragmentTags::GridFragment);
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(Item, FragmentTags::IconFragment);
	if (!GridFragment || !ImageFragment) return;

	UInv_SlottedItemWidget* SlottedItem = CreateSlottedItem(Item, bStackable, StackAmount, GridFragment, ImageFragment, Index);
	AddSlottedItemToCanvas(Index, GridFragment, SlottedItem);

	SlottedItems.Add(Index, SlottedItem);
}

UInv_SlottedItemWidget* UInv_InventoryGridWidget::CreateSlottedItem(UInv_InventoryItem* Item, const bool bStackable,
	const int32 StackAmount, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment,
	const int32 Index)
{
	UInv_SlottedItemWidget* SlottedItem = CreateWidget<UInv_SlottedItemWidget>(GetOwningPlayer(), SlottedItemClass);
	SlottedItem->SetInventoryItem(Item);
	SetSlottedItemImage(SlottedItem, GridFragment, ImageFragment);
	SlottedItem->SetGridIndex(Index);
	SlottedItem->UpdateStackCount(bStackable ? StackAmount : 0);
	SlottedItem->OnSlottedItemClicked.AddDynamic(this, &ThisClass::OnSlottedItemClicked);

	return SlottedItem;
}

void UInv_InventoryGridWidget::AddSlottedItemToCanvas(const int32 Index, const FInv_GridFragment* GridFragment,
	UInv_SlottedItemWidget* SlottedItem) const
{
	CanvasPanel->AddChild(SlottedItem);
	UCanvasPanelSlot* CanvasSlot  = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedItem);
	CanvasSlot->SetSize(GetDrawSize(GridFragment));
	const FVector2D DrawPos = UInv_WidgetUtils::GetPositionFromIndex(Index, Columns) * TileSize;
	const FVector2D DrawPosWithPadding = DrawPos + FVector2D(GridFragment->GetGridPadding());
	CanvasSlot->SetPosition(DrawPosWithPadding);
}

void UInv_InventoryGridWidget::UpdateGridSlots(UInv_InventoryItem* NewItem, const int32 Index, bool bStackableItem, const int32 StackAmount)
{
	check(GridSlots.IsValidIndex(Index));

	if (bStackableItem)
	{
		GridSlots[Index]->SetStackCount(StackAmount);
	}
	
	// 获取格子尺寸
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(NewItem, FragmentTags::GridFragment);
	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);
	
	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns, [&](UInv_GridSlotWidget* GridSlot)
	{
		GridSlot->SetInventoryItem(NewItem);
		GridSlot->SetUpperLeftIndex(Index);
		GridSlot->SetOccupiedTexture();
		GridSlot->SetAvailable(false);
	});
}

bool UInv_InventoryGridWidget::HasRoomAtIndex(const UInv_GridSlotWidget* GridSlot, const FIntPoint& Dimensions,
	const TSet<int32>& CheckedIndices, TSet<int32>& OutTentativelyClaimed, const FGameplayTag& ItemType,int32 MaxStackSize)
{
	bool bHasRoomAtIndex = true;

	UInv_InventoryStatics::ForEach2D(GridSlots, GridSlot->GetIndex(), Dimensions, Columns, [&](const UInv_GridSlotWidget* SubGridSlot)
	{
		if (CheckSlotConstraints(GridSlot, SubGridSlot, CheckedIndices, OutTentativelyClaimed, ItemType, MaxStackSize))
		{
			OutTentativelyClaimed.Add(SubGridSlot->GetIndex());
		}
		else
		{
			bHasRoomAtIndex = false;
		}
	});
	
	return bHasRoomAtIndex;
}

bool UInv_InventoryGridWidget::CheckSlotConstraints(const UInv_GridSlotWidget* GridSlot, const UInv_GridSlotWidget* SubGridSlot,
	const TSet<int32>& CheckedIndices, TSet<int32>& OutTentativelyClaimed, const FGameplayTag& ItemType,int32 MaxStackSize) const
{
	// 索引检查过?
	if (IsIndexClaimed(CheckedIndices, SubGridSlot->GetIndex())) return false;
	
	// 有物品? 没有物品的格子不需要检查
	if (!HasValidItem(SubGridSlot))
	{
		OutTentativelyClaimed.Add(SubGridSlot->GetIndex());
		return true;
	}
	
	// 这个格子是否为左上角（该物品的所占的第一格）?
	if (!IsUpperLeftSlot(GridSlot, SubGridSlot)) return false;
	
	// 物品可重叠?
	UInv_InventoryItem* SubItem = SubGridSlot->GetInventoryItem().Get();
	if (!SubItem->IsStackable()) return false;

	// 是同一类型的物品?
	if (!DoesItemTypeMatched(SubItem, ItemType)) return false;
	
	// 如果可堆叠，是否有足够的堆叠空间?
	if (SubGridSlot->GetStackCount() >= MaxStackSize) return false;
	
	return true;
}

FIntPoint UInv_InventoryGridWidget::GetItemDimensions(const FInv_ItemManifest& Manifest) const
{
	const FInv_GridFragment* GridFragment = Manifest.GetTypeOfFragment<FInv_GridFragment>();
	return GridFragment ? GridFragment->GetGridSize() : FIntPoint(1, 1);
}

bool UInv_InventoryGridWidget::IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const
{
	return CheckedIndices.Contains(Index);
}

bool UInv_InventoryGridWidget::HasValidItem(const UInv_GridSlotWidget* GridSlot) const
{
	return GridSlot->GetInventoryItem().IsValid();
}

bool UInv_InventoryGridWidget::IsUpperLeftSlot(const UInv_GridSlotWidget* GridSlot,
	const UInv_GridSlotWidget* SubGridSlot) const
{
	return SubGridSlot->GetUpperLeftIndex() == GridSlot->GetIndex();
}

bool UInv_InventoryGridWidget::DoesItemTypeMatched(const UInv_InventoryItem* Item, const FGameplayTag& ItemType) const
{
	return Item->GetItemManifest().GetItemType().MatchesTagExact(ItemType);
}

bool UInv_InventoryGridWidget::IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const
{
	if (StartIndex < 0 || StartIndex >= GridSlots.Num()) return false;
	const int32 EndColumn = (StartIndex % Columns) + ItemDimensions.X;
	const int32 EndRow = (StartIndex / Columns) + ItemDimensions.Y;
	return EndColumn <= Columns && EndRow <= Rows;
}

int32 UInv_InventoryGridWidget::DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize,
	const int32 AmountToFill, const UInv_GridSlotWidget* GridSlot) const
{
	// 获取槽位剩余空间，填入数量小于则正常填入，否则填满
	const int32 RoomInSlot = MaxStackSize - GetStackAmount(GridSlot);
	return bStackable ? FMath::Min(AmountToFill, RoomInSlot) : 1;
}

int32 UInv_InventoryGridWidget::GetStackAmount(const UInv_GridSlotWidget* GridSlot) const
{
	int32 CurrentSlotStackCount = GridSlot->GetStackCount();
	// 如果我们在一个不保存堆叠计数的槽位上，我们必须获取实际的堆叠计数.
	if (const int32 UpperLeftIndex = GridSlot->GetUpperLeftIndex(); UpperLeftIndex != INDEX_NONE)
	{
		UInv_GridSlotWidget* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
		CurrentSlotStackCount = UpperLeftGridSlot->GetStackCount();
	}
	return CurrentSlotStackCount;
}

void UInv_InventoryGridWidget::AddStacks(const FInv_SlotAvailabilityResult& Result)
{
	if (!MatchesCategory(Result.Item.Get())) return;

	for (const auto& Availability : Result.SlotAvailabilities)
	{
		if (Availability.bItemAtIndex)
		{
			const auto& GridSlot = GridSlots[Availability.Index];
			const auto& SlottedItem = SlottedItems.FindChecked(Availability.Index);
			SlottedItem->UpdateStackCount(GridSlot->GetStackCount() + Availability.AmountToFill);
			GridSlot->SetStackCount(GridSlot->GetStackCount() + Availability.AmountToFill);
		}
		else
		{
			AddItemAtIndex(Result.Item.Get(), Availability.Index, Result.bStackable, Availability.AmountToFill);
			UpdateGridSlots(Result.Item.Get(), Availability.Index, Result.bStackable, Availability.AmountToFill);
		}
	}
}

bool UInv_InventoryGridWidget::IsRightClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}

bool UInv_InventoryGridWidget::IsLeftClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}

void UInv_InventoryGridWidget::OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	check(GridSlots.IsValidIndex(GridIndex));
	UInv_InventoryItem* ClickedInventoryItem  = GridSlots[GridIndex]->GetInventoryItem().Get();
	
	if (!IsValid(HoverItem) && IsLeftClick(MouseEvent))
	{
		PickUp(ClickedInventoryItem, GridIndex);
	}
}

void UInv_InventoryGridWidget::PickUp(UInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	AssignHoverItem(ClickedInventoryItem, GridIndex, GridIndex);
	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);
}

void UInv_InventoryGridWidget::AssignHoverItem(UInv_InventoryItem* InventoryItem, const int32 GridIndex,
	const int32 PreviousGridIndex)
{
	AssignHoverItem(InventoryItem);

	HoverItem->UpdateStackCount(HoverItem->IsStackable() ? GridSlots[GridIndex]->GetStackCount() : 0);
	HoverItem->SetPreviousGridIndex(PreviousGridIndex);
}

void UInv_InventoryGridWidget::AssignHoverItem(UInv_InventoryItem* InventoryItem)
{
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(InventoryItem, FragmentTags::GridFragment);
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(InventoryItem, FragmentTags::IconFragment);
	if (!GridFragment || !ImageFragment) return;

	HoverItem = CreateWidget<UInv_HoverItemWidget>(this, HoverItemClass);
	const FVector2D DrawSize = GetDrawSize(GridFragment);
	FSlateBrush IconBrush;
	IconBrush.SetResourceObject(ImageFragment->GetIcon());
	IconBrush.DrawAs = ESlateBrushDrawType::Image;
	IconBrush.ImageSize = DrawSize * UWidgetLayoutLibrary::GetViewportScale(this); // 图形大小要乘缩放DPI

	HoverItem->SetImageBrush(IconBrush);
	HoverItem->SetGridDimensions(GridFragment->GetGridSize());
	HoverItem->SetInventoryItem(InventoryItem);
	HoverItem->SetIsStackable(InventoryItem->IsStackable());

	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, HoverItem);
}

void UInv_InventoryGridWidget::RemoveItemFromGrid(UInv_InventoryItem* InventoryItem, const int32 GridIndex)
{
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(InventoryItem, FragmentTags::GridFragment);
	if (!GridFragment) return;

	UInv_InventoryStatics::ForEach2D(GridSlots, GridIndex, GridFragment->GetGridSize(), Columns, [&](UInv_GridSlotWidget* GridSlot)
	{
		GridSlot->SetInventoryItem(nullptr);
		GridSlot->SetUpperLeftIndex(INDEX_NONE);
		GridSlot->SetUnoccupiedTexture();
		GridSlot->SetAvailable(true);
		GridSlot->SetStackCount(0);
	});

	if (SlottedItems.Contains(GridIndex))
	{
		TObjectPtr<UInv_SlottedItemWidget> FoundSlottedItem;
		SlottedItems.RemoveAndCopyValue(GridIndex, FoundSlottedItem);
		FoundSlottedItem->RemoveFromParent();
	}
}

FVector2D UInv_InventoryGridWidget::GetDrawSize(const FInv_GridFragment* GridFragment) const
{
	// 计算图标的尺寸
	const float IconTileWidth = TileSize - GridFragment->GetGridPadding() * 2;	// 图标宽度等于格子大小减去间距
	return GridFragment->GetGridSize() * IconTileWidth;	// 格子尺寸乘以图标宽度
}

void UInv_InventoryGridWidget::SetSlottedItemImage(const UInv_SlottedItemWidget* SlottedItem,
	const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment) const
{
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = GetDrawSize(GridFragment);
	SlottedItem->SetImageBrush(Brush);
}

void UInv_InventoryGridWidget::ConstructGrid()
{
	// 提前分配内存
	GridSlots.Reserve(Rows * Columns);
	
	for (int32 i = 0; i < Rows; ++i)
	{
		for (int32 j = 0; j < Columns; ++j)
		{
			UInv_GridSlotWidget* GridSlot = CreateWidget<UInv_GridSlotWidget>(this, GridSlotClass);
			CanvasPanel->AddChild(GridSlot);

			const FIntPoint TilePosition = FIntPoint(j, i);
			GridSlot->SetTileIndex(UInv_WidgetUtils::GetIndexFromPosition(TilePosition, Columns));

			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCPS->SetSize(FVector2D(TileSize));
			GridCPS->SetPosition(TilePosition * TileSize);

			GridSlots.Add(GridSlot);
		}
	}
}

bool UInv_InventoryGridWidget::MatchesCategory(const UInv_InventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}


