// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Spatial/Inv_InventoryGridWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlotWidget.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"

void UInv_InventoryGridWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();
}

void UInv_InventoryGridWidget::ConstructGrid()
{
	// 提前分配内存
	GridSlots.Reserve(Rows * Columns);
	
	for (int32 i = 0; i < Rows; ++i)
	{
		for (int32 j = 0; j < Columns; ++j)
		{
			UInv_GridSlotWidget* GridSlot = CreateWidget<UInv_GridSlotWidget>(this, GridSlotClass);
			CanvasPanel->AddChild(GridSlot);

			const FIntPoint TilePosition = FIntPoint(j, i);
			GridSlot->SetTileIndex(UInv_WidgetUtils::GetIndexFromPosition(TilePosition, Columns));

			UCanvasPanelSlot* GridCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCPS->SetSize(FVector2D(TileSize));
			GridCPS->SetPosition(TilePosition * TileSize);

			GridSlots.Add(GridSlot);
		}
	}
}
