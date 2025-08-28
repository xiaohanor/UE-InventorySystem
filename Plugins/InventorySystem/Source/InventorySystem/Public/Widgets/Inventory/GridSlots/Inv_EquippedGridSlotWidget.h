// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inv_GridSlotWidget.h"
#include "Inv_EquippedGridSlotWidget.generated.h"

class UOverlay;
class UInv_EquippedSlottedItemWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquippedGridSlotClicked, UInv_EquippedGridSlotWidget*, GridSlot,
                                             const FGameplayTag&, EquipmentTypeTag);

/**
 * 角色装备格子
 */
UCLASS()
class INVENTORYSYSTEM_API UInv_EquippedGridSlotWidget : public UInv_GridSlotWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	UInv_EquippedSlottedItemWidget* OnItemEquipped(UInv_InventoryItem* Item, const FGameplayTag& EquipmentTag, float TileSize);

	FEquippedGridSlotClicked EquippedGridSlotClicked;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GrayedOutIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Overlay_Root;
	
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories = "GameItems.Equipment"))
	FGameplayTag EquipmentTypeTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_EquippedSlottedItemWidget> EquippedSlottedItemWidgetClass;

	UPROPERTY()
	TObjectPtr<UInv_EquippedSlottedItemWidget> EquippedSlottedItemWidget;
};
