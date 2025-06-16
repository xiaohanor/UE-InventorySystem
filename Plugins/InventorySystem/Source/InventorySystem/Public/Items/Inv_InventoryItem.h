// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Inv_InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInv_InventoryItem : public UObject
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetItemManifest(const FInv_ItemManifest& Manifest);
	
private:
	// FInstancedStruct 能够在同一个变量里，持有不同类型的结构体数据
	// meta = (BaseStruct = "/Script/InventorySystem.Inv_ItemManifest") 这个元数据标签非常有用！
	// 它告诉编辑器，当我们设置 FInstancedStruct 时，只在下拉列表里显示那些与 Inv_ItemManifest “相关”的结构体，让界面更整洁。
	UPROPERTY(VisibleAnywhere, meta = (BaseStruct = "/Script/InventorySystem.Inv_ItemManifest"), Replicated)
	FInstancedStruct ItemManifest;
	
};
