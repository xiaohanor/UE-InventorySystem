#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Types/Inv_GridTypes.h"
#include "StructUtils/InstancedStruct.h"

#include "Inv_ItemManifest.generated.h"

/**
 * Item Manifest 包含所有创建新库存物品的必要数据
 */

struct FInv_ItemFragment;
class UInv_InventoryItem;

USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FInv_ItemManifest
{
	GENERATED_BODY()

	UInv_InventoryItem* Manifest(UObject* NewOuter);
	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }
	FGameplayTag GetItemType() const { return ItemType; }

	// 通过Tag获取碎片类型
	template<typename T>
	requires std::derived_from<T, FInv_ItemFragment>
	const T* GetTypeOfFragmentWithTag(const FGameplayTag& Tag) const;

	template<typename T>
	requires std::derived_from<T, FInv_ItemFragment>
	const T* GetFragmentOfType() const;

	template<typename T>
	requires std::derived_from<T, FInv_ItemFragment>
	T* GetFragmentOfTypeMutable();

private:
	// 物品碎片
	// meta = (ExcludeBaseStruct) 编辑时不包含基类
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FInv_ItemFragment>> Fragments;
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	EInv_ItemCategory ItemCategory{EInv_ItemCategory::None};

	UPROPERTY(EditAnywhere, meta = (Categories = "GameItems"),Category = "Inventory")
	FGameplayTag ItemType;
};

template<typename T>
	requires std::derived_from<T, FInv_ItemFragment>
	const T* FInv_ItemManifest::GetTypeOfFragmentWithTag(const FGameplayTag& Tag) const
{
	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			if (!FragmentPtr->GetFragmentTag().MatchesTagExact(Tag)) continue;	// Tag不匹配就跳过
			return FragmentPtr;
		}
	}
	
	return nullptr;
}

template<typename T>
	requires std::derived_from<T, FInv_ItemFragment>
	const T* FInv_ItemManifest::GetFragmentOfType() const
{
	for (const TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			return FragmentPtr;
		}
	}
	
	return nullptr;
}

template<typename T>
	requires std::derived_from<T, FInv_ItemFragment>
	T* FInv_ItemManifest::GetFragmentOfTypeMutable()
{
	for (TInstancedStruct<FInv_ItemFragment>& Fragment : Fragments)
	{
		if (T* FragmentPtr = Fragment.GetMutablePtr<T>())
		{
			return FragmentPtr;
		}
	}
	
	return nullptr;
}

