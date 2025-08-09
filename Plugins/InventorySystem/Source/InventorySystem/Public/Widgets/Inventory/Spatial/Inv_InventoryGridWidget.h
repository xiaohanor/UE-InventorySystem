// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryGridWidget.generated.h"

class UInv_HoverItemWidget;
struct FGameplayTag;
struct FInv_ImageFragment;
class UInv_SlottedItemWidget;
struct FInv_GridFragment;
struct FInv_ItemManifest;
class UInv_ItemComponent;
class UInv_InventoryComponent;
class UCanvasPanel;
class UInv_GridSlotWidget;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInv_InventoryGridWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }
	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_ItemComponent* ItemComponent);

	UFUNCTION()
	void AddItem(UInv_InventoryItem* Item);

private:
	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;
	
	void ConstructGrid();
	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_InventoryItem* Item);
	FInv_SlotAvailabilityResult HasRoomForItem(const FInv_ItemManifest& Manifest);
	void AddItemToIndices(const FInv_SlotAvailabilityResult& Result, UInv_InventoryItem* NewItem);
	bool MatchesCategory(const UInv_InventoryItem* Item) const;
	FVector2D GetDrawSize(const FInv_GridFragment* GridFragment) const;
	void SetSlottedItemImage(const UInv_SlottedItemWidget* SlottedItem, const FInv_GridFragment* GridFragment, const FInv_ImageFragment* ImageFragment) const;
	void AddItemAtIndex(UInv_InventoryItem* Item, const int32 Index, const bool bStackable, const int32 StackAmount);
	UInv_SlottedItemWidget* CreateSlottedItem(UInv_InventoryItem* Item,
		const bool bStackable,
		const int32 StackAmount,
		const FInv_GridFragment* GridFragment,
		const FInv_ImageFragment* ImageFragment,
		const int32 Index);
	void AddSlottedItemToCanvas(const int32 Index, const FInv_GridFragment* GridFragment, UInv_SlottedItemWidget* SlottedItem) const;
	void UpdateGridSlots(UInv_InventoryItem* NewItem, const int32 Index, bool bStackableItem, const int32 StackAmount);
	bool HasRoomAtIndex(const UInv_GridSlotWidget* GridSlot,
		const FIntPoint& Dimensions,
		const TSet<int32>& CheckedIndices,
		TSet<int32>& OutTentativelyClaimed,
		const FGameplayTag& ItemType,
		int32 MaxStackSize);
	bool CheckSlotConstraints(const UInv_GridSlotWidget* GridSlot,
		const UInv_GridSlotWidget* SubGridSlot,
		const TSet<int32>& CheckedIndices,
		TSet<int32>& OutTentativelyClaimed,
		const FGameplayTag& ItemType,
		int32 MaxStackSize) const;
	FIntPoint GetItemDimensions(const FInv_ItemManifest& Manifest) const;
	bool IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const;
	bool HasValidItem(const UInv_GridSlotWidget* GridSlot) const;
	bool IsUpperLeftSlot(const UInv_GridSlotWidget* GridSlot, const UInv_GridSlotWidget* SubGridSlot) const;
	bool DoesItemTypeMatched(const UInv_InventoryItem* Item, const FGameplayTag& ItemType) const;
	bool IsInGridBounds(int32 StartIndex, const FIntPoint& ItemDimensions) const;
	int32 DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize, const int32 AmountToFill, const UInv_GridSlotWidget* GridSlot) const;
	int32 GetStackAmount(const UInv_GridSlotWidget* GridSlot) const;
	bool IsRightClick(const FPointerEvent& MouseEvent) const;
	bool IsLeftClick(const FPointerEvent& MouseEvent) const;
	void PickUp(UInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex);
	void AssignHoverItem(UInv_InventoryItem* InventoryItem);
	void AssignHoverItem(UInv_InventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviousGridIndex);
	void RemoveItemFromGrid(UInv_InventoryItem* InventoryItem, const int32 GridIndex);
	void UpdateTileParameters(const FVector2D& CanvasPosition, const FVector2D& MousePosition);
	FIntPoint CalculateHoveredCoordinates(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;

	UFUNCTION()
	void AddStacks(const FInv_SlotAvailabilityResult& Result);

	UFUNCTION()
	void OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),  Category = "Inventory")
	EInv_ItemCategory ItemCategory;

	// 画布面板
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	/* 网格插槽 */  
	UPROPERTY()
	TArray<TObjectPtr<UInv_GridSlotWidget>> GridSlots;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_GridSlotWidget> GridSlotClass;
	/* 网格插槽 */

	/* 插槽内物品控件 */
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_SlottedItemWidget> SlottedItemClass;

	UPROPERTY()
	TMap<int32, TObjectPtr<UInv_SlottedItemWidget>> SlottedItems;
	/* 插槽内物品控件 */

	/* 物品悬停控件 */
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_HoverItemWidget> HoverItemClass;

	UPROPERTY()
	TObjectPtr<UInv_HoverItemWidget> HoverItem;
	/* 物品悬停控件 */
	
	// 行数
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Rows;

	// 列数
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Columns;

	// 每个格子的尺寸
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float TileSize;

	FInv_TileParameters TileParameters;
	FInv_TileParameters LastTileParameters;
};
