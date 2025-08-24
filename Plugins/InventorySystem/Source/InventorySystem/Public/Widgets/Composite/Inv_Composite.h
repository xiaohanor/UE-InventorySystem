// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inv_CompositeBase.h"
#include "Inv_Composite.generated.h"

/**
 * 组合节点：持有多个子控件，承担聚合与递归操作（组合模式）
 */
UCLASS()
class INVENTORYSYSTEM_API UInv_Composite : public UInv_CompositeBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void ApplyFunction(FuncType Function) override;
	virtual void Collapse() override;
	
private:
	UPROPERTY()
	TArray<TObjectPtr<UInv_CompositeBase>> Children;
};
