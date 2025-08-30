// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Component/Inv_EquipmentComponent.h"

#include "GameFramework/Character.h"
#include "InventoryManagement/Component/Inv_InventoryComponent.h"
#include "InventoryManagement/EquipActor/Inv_EquipActor.h"
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

AInv_EquipActor* UInv_EquipmentComponent::SpawnEquippedActor(FInv_EquipmentFragment* EquipmentFragment,
	const FInv_ItemManifest& Manifest, USkeletalMeshComponent* AttachMesh)
{
	AInv_EquipActor* SpawnedEquipActor = EquipmentFragment->SpawnAttachedActor(AttachMesh);
	SpawnedEquipActor->SetEquipmentType(EquipmentFragment->GetEquipmentType()); //已经在蓝图设置了，为什么这里要再设置一次？
	SpawnedEquipActor->SetOwner(GetOwner());
	EquipmentFragment->SetEquippedActor(SpawnedEquipActor);
	return SpawnedEquipActor;
}

void UInv_EquipmentComponent::OnItemEquipped(UInv_InventoryItem* EquippedItem)
{
	if (!IsValid(EquippedItem)) return;
	if (!OwningPlayerController->HasAuthority()) return;

	FInv_ItemManifest& ItemManifest = EquippedItem->GetItemManifestMutable();
	FInv_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FInv_EquipmentFragment>();
	if (!EquipmentFragment) return;

	EquipmentFragment->OnEquip(OwningPlayerController.Get());

	if (!OwningSkeletalMesh.IsValid()) return;
	AInv_EquipActor* SpawnedEquipActor = SpawnEquippedActor(EquipmentFragment, ItemManifest, OwningSkeletalMesh.Get()); // 这个Manifest有必要吗？

	EquippedActors.Add(SpawnedEquipActor);
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



