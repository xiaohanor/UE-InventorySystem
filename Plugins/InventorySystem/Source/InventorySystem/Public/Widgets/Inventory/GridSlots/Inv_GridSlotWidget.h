﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlotWidget.generated.h"

class UInv_InventoryItem;
class UImage;
/**
 * 
 */

UENUM()
enum EInv_GridSlotState : uint8
{
	Unoccupied,	// 空
	Occupied,	// 填充
	Selected,	// 选中
	GrayedOut	// 灰色
};

UCLASS()
class INVENTORYSYSTEM_API UInv_GridSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetTileIndex(const int32 Index) { TileIndex = Index; }
	int32 GetTileIndex() const { return TileIndex; }
	EInv_GridSlotState GetGridSlotState() const { return GridSlotState; }
	TWeakObjectPtr<UInv_InventoryItem> GetInventoryItem() const { return InventoryItem; }
	void SetInventoryItem(UInv_InventoryItem* Item);
	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 Count) { StackCount = Count; }
	int32 GetIndex() const { return TileIndex; }
	void SetIndex(int32 Index) { TileIndex = Index; }
	int32 GetUpperLeftIndex() const { return UpperLeftIndex; }
	void SetUpperLeftIndex(int32 Index) { UpperLeftIndex = Index; }
	bool IsAvailable() const { return bAvailable; }
	void SetAvailable(bool bIsAvailable) { bAvailable = bIsAvailable; }
	
	void SetOccupiedTexture();
	void SetUnoccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();

private:
	int32 TileIndex;
	int32 StackCount;
	int32 UpperLeftIndex{INDEX_NONE};
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;
	bool bAvailable;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FSlateBrush Brush_Unoccupied;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FSlateBrush Brush_Occupied;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FSlateBrush Brush_Selected;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FSlateBrush Brush_GrayedOut;

	EInv_GridSlotState GridSlotState;
};
