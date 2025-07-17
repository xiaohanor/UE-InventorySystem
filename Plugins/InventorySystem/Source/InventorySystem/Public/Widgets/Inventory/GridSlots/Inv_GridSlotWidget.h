// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlotWidget.generated.h"

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
	
	void SetOccupiedTexture();
	void SetUnoccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();

private:
	int32 TileIndex;

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
