// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Inv_CompositeBase.generated.h"

/**
 * 用于组合 ItemDescription 的基础控件
 * 作为组合模式的抽象基类，定义叶子与组合容器共享的公共接口
 */
UCLASS()
class INVENTORYSYSTEM_API UInv_CompositeBase : public UUserWidget
{
	GENERATED_BODY()

public:
	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(const FGameplayTag& Tag) { FragmentTag = Tag; }
	virtual void Collapse();
	void Expand();

	/** 应用于节点的函数签名，参数为当前节点指针 */
	using FuncType = TFunction<void(UInv_CompositeBase*)>;
	/**
	 * 对当前节点（及必要时对子树）应用传入函数。
	 * 基类默认不做任何事，供子类覆盖实现。
	 */
	virtual void ApplyFunction(FuncType Function) {}
	
private:
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories = "FragmentTags"))
	FGameplayTag FragmentTag;
};
