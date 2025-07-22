﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inv_InventoryItem.h"

#include "Items/Fragments/Inv_ItemFragment.h"
#include "Net/UnrealNetwork.h"

void UInv_InventoryItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
}

void UInv_InventoryItem::SetItemManifest(const FInv_ItemManifest& Manifest)
{
	ItemManifest = FInstancedStruct::Make<FInv_ItemManifest>(Manifest);
}

bool UInv_InventoryItem::IsStackable() const
{
	const FInv_StackableFragment* StackableFragment = GetItemManifest().GetTypeOfFragment<FInv_StackableFragment>();
	return StackableFragment != nullptr;
}
