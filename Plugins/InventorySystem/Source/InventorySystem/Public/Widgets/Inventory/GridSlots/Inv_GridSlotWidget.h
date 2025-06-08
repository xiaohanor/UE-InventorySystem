// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlotWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInv_GridSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetTileIndex(const int32 Index) { TileIndex = Index; }
	int32 GetTileIndex() const { return TileIndex; }

private:
	int32 TileIndex;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;
};
