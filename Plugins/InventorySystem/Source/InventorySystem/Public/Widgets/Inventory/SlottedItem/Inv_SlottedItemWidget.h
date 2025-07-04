﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_SlottedItemWidget.generated.h"

class UInv_InventoryItem;
class UImage;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInv_SlottedItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	bool IsStackable() const { return bIsStackable; }
	void SetIsStackable(bool bStackable) { bIsStackable = bStackable; }
	UImage* GetImageIcon() const { return Image_Icon; }
	void SetGridIndex(int32 Index) { GridIndex = Index; }
	int32 GetGridIndex() const { return GridIndex; }
	void SetGridDimensions(const FIntPoint& Dimensions) { GridDimensions = Dimensions; }
	FIntPoint GetGridDimensions() const { return GridDimensions; }
	void SetInventoryItem(UInv_InventoryItem* Item);
	UInv_InventoryItem* GetInventoryItem() const { return InventoryItem.Get(); }
	void SetImageBrush(const FSlateBrush& Brush) const;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	// 网格索引
	int32 GridIndex;
	// 网格尺寸
	FIntPoint GridDimensions;
	// 物品
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;
	// 是否可堆叠
	bool bIsStackable{false};
};
