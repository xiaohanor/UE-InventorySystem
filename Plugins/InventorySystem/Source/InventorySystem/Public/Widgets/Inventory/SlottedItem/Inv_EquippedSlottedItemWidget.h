// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inv_SlottedItemWidget.h"
#include "Inv_EquippedSlottedItemWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquippedSlottedItemClicked, class UInv_EquippedSlottedItemWidget*, SlottedItem);

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInv_EquippedSlottedItemWidget : public UInv_SlottedItemWidget
{
	GENERATED_BODY()

public:  
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;  
	void SetEquipmentTypeTag(const FGameplayTag& Tag) { EquipmentTypeTag = Tag; }  
	FGameplayTag GetEquipmentTypeTag() const { return EquipmentTypeTag; }  
  
	FEquippedSlottedItemClicked OnEquippedSlottedItemClicked;
	
private:  
	UPROPERTY()  
	FGameplayTag EquipmentTypeTag;  
};
