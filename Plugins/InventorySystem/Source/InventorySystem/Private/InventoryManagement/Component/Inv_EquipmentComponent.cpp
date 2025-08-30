// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Component/Inv_EquipmentComponent.h"

#include "GameFramework/Character.h"
#include "InventoryManagement/Component/Inv_InventoryComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Items/Manifest/Inv_ItemManifest.h"


void UInv_EquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningPlayerController = Cast<APlayerController>(GetOwner());
	if (OwningPlayerController.IsValid())
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwningPlayerController->GetPawn()); IsValid(OwnerCharacter))
		{
			OwningSkeletalMesh = OwnerCharacter->GetMesh();
		}
		InitInventoryComponent();
	}
}

void UInv_EquipmentComponent::InitInventoryComponent()
{
	InventoryComponent = OwningPlayerController->FindComponentByClass<UInv_InventoryComponent>();
	if (!InventoryComponent.IsValid()) return;

	if (!InventoryComponent->OnItemEquipped.IsAlreadyBound(this, &ThisClass::UInv_EquipmentComponent::OnItemEquipped))
	{
		InventoryComponent->OnItemEquipped.AddDynamic(this, &ThisClass::UInv_EquipmentComponent::OnItemEquipped);
	}
	if (!InventoryComponent->OnItemUnequipped.IsAlreadyBound(this, &ThisClass::UInv_EquipmentComponent::OnItemUnequipped))
	{
		InventoryComponent->OnItemUnequipped.AddDynamic(this, &ThisClass::UInv_EquipmentComponent::OnItemUnequipped);
	}
}

void UInv_EquipmentComponent::OnItemEquipped(UInv_InventoryItem* EquippedItem)
{
	if (!IsValid(EquippedItem)) return;
	if (!OwningPlayerController->HasAuthority()) return;

	FInv_ItemManifest& ItemManifest = EquippedItem->GetItemManifestMutable();
	FInv_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FInv_EquipmentFragment>();
	if (!EquipmentFragment) return;

	EquipmentFragment->OnEquip(OwningPlayerController.Get());
}

void UInv_EquipmentComponent::OnItemUnequipped(UInv_InventoryItem* UnequippedItem)
{
	if (!IsValid(UnequippedItem)) return;
	if (!OwningPlayerController->HasAuthority()) return;

	FInv_ItemManifest& ItemManifest = UnequippedItem->GetItemManifestMutable();
	FInv_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FInv_EquipmentFragment>();
	if (!EquipmentFragment) return;

	EquipmentFragment->OnUnequip(OwningPlayerController.Get());
}



