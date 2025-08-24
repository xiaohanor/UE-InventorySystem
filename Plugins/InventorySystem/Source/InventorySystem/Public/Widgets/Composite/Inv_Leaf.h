// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inv_CompositeBase.h"
#include "Inv_Leaf.generated.h"

/**
 * 叶子节点：不包含子元素，仅代表一个最小的展示或行为单元
 */
UCLASS()
class INVENTORYSYSTEM_API UInv_Leaf : public UInv_CompositeBase
{
	GENERATED_BODY()

public:
	virtual void ApplyFunction(FuncType Function) override;
};
