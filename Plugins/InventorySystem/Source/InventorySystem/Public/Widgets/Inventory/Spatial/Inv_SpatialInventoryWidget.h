// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBaseWidget.h"
#include "Inv_SpatialInventoryWidget.generated.h"

struct FGameplayTag;
class UInv_EquippedGridSlotWidget;
class UInv_ItemDescription;
class UCanvasPanel;
class UButton;
class UWidgetSwitcher;
class UInv_InventoryGridWidget;
/**
 * 库存的主页面，包含不同类型物品储存空间（UInv_InventoryGridWidget）
 */
UCLASS()
class INVENTORYSYSTEM_API UInv_SpatialInventoryWidget : public UInv_InventoryBaseWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
	virtual FInv_SlotAvailabilityResult HasRoomForItem(UInv_ItemComponent* ItemComponent) const override;
	virtual void OnItemHovered(UInv_InventoryItem* Item) override;
	virtual void OnItemUnHovered() override;
	virtual bool HasHoverItem() const override;
	virtual UInv_HoverItemWidget* GetHoverItem() const override;
	
private:
	TWeakObjectPtr<UInv_InventoryGridWidget> ActiveGrid;

	UPROPERTY()
	TArray<TObjectPtr<UInv_EquippedGridSlotWidget>> EquippedGridSlots;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGridWidget> Grid_Equippables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGridWidget> Grid_Consumables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGridWidget> Grid_Craftables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Equippables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Consumables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Craftables;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_ItemDescription> ItemDescriptionClass;

	UPROPERTY()
	TObjectPtr<UInv_ItemDescription> ItemDescription;

	FTimerHandle DescriptionTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	float DescriptionTimerDelay = 0.5f;

	UInv_ItemDescription* GetItemDescription();

	UFUNCTION()
	void ShowEquippables();

	UFUNCTION()
	void ShowConsumables();

	UFUNCTION()
	void ShowCraftables();

	UFUNCTION()
	void EquippedGridSlotClicked(UInv_EquippedGridSlotWidget* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag);
	
	void DisableButton(UButton* Button);
	void SetActiveGrid(UInv_InventoryGridWidget* Grid, UButton* Button);
	void SetItemDescriptionSizeAndPosition(UInv_ItemDescription* Description, UCanvasPanel* Canvas) const;
};
