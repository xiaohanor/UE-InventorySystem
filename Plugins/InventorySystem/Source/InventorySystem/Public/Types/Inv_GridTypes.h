#pragma once

#include "Inv_GridTypes.generated.h"

class UInv_InventoryItem;

UENUM(BlueprintType)
enum class EInv_ItemCategory : uint8
{
	// 装备
	Equippable,
	// 消耗品
	Consumable,
	// 材料
	Craftable,
	None
};

USTRUCT()
struct FInv_SlotAvailability
{
	GENERATED_BODY()

	FInv_SlotAvailability() {}
	FInv_SlotAvailability(int32 ItemIndex, int32 Room, bool bHasItem) : Index(ItemIndex), AmountToFill(Room), bItemAtIndex(bHasItem) {}

	// 库存插槽的索引
	int32 Index{INDEX_NONE};
	// 剩余容量
	int32 AmountToFill{0};
	// 是否有物品
	bool bItemAtIndex{false};
};

USTRUCT()
struct FInv_SlotAvailabilityResult
{
	GENERATED_BODY()

	FInv_SlotAvailabilityResult() {}

	// 物品
	TWeakObjectPtr<UInv_InventoryItem> Item;
	// 库存总容量
	int32 TotalRoomToFill{0};
	// 堆叠物品剩余容量
	int32 Remainder{0};
	// 是否可堆叠
	bool bStackable{false};
	TArray<FInv_SlotAvailability> SlotAvailabilities;
};
