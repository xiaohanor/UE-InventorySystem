#pragma once

#include "Inv_GridTypes.generated.h"

class UInv_InventoryItem;

/*
 * 物品类别
 */
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

/*
 * 表示单个网格槽位的可用性信息
 */
USTRUCT()
struct FInv_SlotAvailability
{
	GENERATED_BODY()

	FInv_SlotAvailability() {}
	FInv_SlotAvailability(int32 ItemIndex, int32 Room, bool bHasItem) : Index(ItemIndex), AmountToFill(Room), bItemAtIndex(bHasItem) {}

	// 插槽的索引位置
	int32 Index{INDEX_NONE};
	// 该槽位可以填充的物品数量
	int32 AmountToFill{0};
	// 该索引位置是否已有物品
	bool bItemAtIndex{false};
};

/*
 * 包含完整的物品放置查询结果
 */
USTRUCT()
struct FInv_SlotAvailabilityResult
{
	GENERATED_BODY()

	FInv_SlotAvailabilityResult() {}

	// 相关的库存物品引用
	TWeakObjectPtr<UInv_InventoryItem> Item;
	// 总共可以填充的物品数量
	int32 TotalRoomToFill{0};
	// 无法放置的剩余数量
	int32 Remainder{0};
	// 物品是否可堆叠
	bool bStackable{false};
	// 所有可用槽位的详细信息数组
	TArray<FInv_SlotAvailability> SlotAvailabilities;
};


/**
 * 图块象限
 */
UENUM(BlueprintType)
enum class EInv_TileQuadrant : uint8
{
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
	None
};


/**
 * 包含图块坐标、索引和象限
 */
USTRUCT(BlueprintType)
struct FInv_TileParameters
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FIntPoint TileCoordinates{};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 TileIndex{INDEX_NONE};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	EInv_TileQuadrant TileQuadrant{EInv_TileQuadrant::None};
};

inline bool operator==(const FInv_TileParameters& A, const FInv_TileParameters& B)
{
	return A.TileCoordinates == B.TileCoordinates && A.TileIndex == B.TileIndex && A.TileQuadrant == B.TileQuadrant;
}

/**
 * 空间查询结果
 */
USTRUCT()
struct FInv_SpaceQueryResult
{
	GENERATED_BODY()
	
	// 如果查询的空间中没有物品则为True
	bool bHasSpace{false};

	// 如果只有一个可交换的项，则有效
	TWeakObjectPtr<UInv_InventoryItem> ValidItem = nullptr;

	// 有效项的左上角索引（如果有）
	int32 UpperLeftIndex{INDEX_NONE};
};