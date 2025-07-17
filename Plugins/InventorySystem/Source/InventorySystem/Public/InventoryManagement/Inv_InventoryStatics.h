// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/Inv_GridTypes.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "Inv_InventoryStatics.generated.h"

class UInv_ItemComponent;
class UInv_InventoryComponent;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInv_InventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UInv_InventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static EInv_ItemCategory GetItemCategoryFromItemComp(UInv_ItemComponent* ItemComp);

	template<typename T, typename FunctT>
	static void ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 Columns, const FunctT& Function);
};

template <typename T, typename FunctT>
void UInv_InventoryStatics::ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 Columns,
	const FunctT& Function)
{
	for (int32 i = 0; i < Range2D.Y; ++i)
	{
		for (int32 j = 0; j < Range2D.X; ++j)
		{
			const FIntPoint Coordinate = UInv_WidgetUtils::GetPositionFromIndex(Index, Columns) + FIntPoint(j, i);
			const int32 ArrayIndex = UInv_WidgetUtils::GetIndexFromPosition(Coordinate, Columns);
			if (Array.IsValidIndex(ArrayIndex))
			{
				Function(Array[ArrayIndex]);
			}
		}
	}
}
