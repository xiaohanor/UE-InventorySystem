// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryBaseWidget.generated.h"

class UInv_HoverItemWidget;
class UInv_ItemComponent;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInv_InventoryBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual FInv_SlotAvailabilityResult HasRoomForItem(UInv_ItemComponent* ItemComponent) const { return FInv_SlotAvailabilityResult(); }
	virtual void OnItemHovered(UInv_InventoryItem* Item) {}
	virtual void OnItemUnHovered() {}
	virtual bool HasHoverItem() const { return false; }
	virtual UInv_HoverItemWidget* GetHoverItem() const { return nullptr; }
};
