// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBaseWidget.h"
#include "Inv_SpatialInventoryWidget.generated.h"

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

	virtual FInv_SlotAvailabilityResult HasRoomForItem(UInv_ItemComponent* ItemComponent) const override; 
	
private:
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

	UFUNCTION()
	void ShowEquippables();

	UFUNCTION()
	void ShowConsumables();

	UFUNCTION()
	void ShowCraftables();
	
	void DisableButton(UButton* Button);
	void SetActiveGrid(UInv_InventoryGridWidget* Grid, UButton* Button);
	TWeakObjectPtr<UInv_InventoryGridWidget> ActiveGrid;
};
