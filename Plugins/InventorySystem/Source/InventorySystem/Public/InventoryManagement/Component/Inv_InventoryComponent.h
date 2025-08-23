// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryManagement/FastArray/Inv_FastArray.h"
#include "Inv_InventoryComponent.generated.h"

struct FInv_SlotAvailabilityResult;
class UInv_ItemComponent;
class UInv_InventoryItem;
class UInv_InventoryBaseWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChanged, UInv_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackChange, const FInv_SlotAvailabilityResult&, Result);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORYSYSTEM_API UInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_InventoryComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void Server_AddNewItem(UInv_ItemComponent* ItemComponent, int32 StackCount);

	UFUNCTION(Server, Reliable)
	void Server_AddStacksToItem(UInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	UFUNCTION(Server, Reliable)
	void Server_DropItem(UInv_InventoryItem* Item, int32 StackCount);

	UFUNCTION(Server, Reliable)
	void Server_ConsumeItem(UInv_InventoryItem* Item);

	void ToggleInventoryMenu();
	// 将物品作为子对象进行网络同步
	void AddRepSubObj(UObject* SubObj);
	void SpawnDroppedItem(UInv_InventoryItem* Item, int32 StackCount);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	void TryAddItem(UInv_ItemComponent* ItemComponent);

	FInventoryItemChanged OnItemAdded;
	FInventoryItemChanged OnItemRemoved;
	FNoRoomInInventory NoRoomInInventory;
	FStackChange OnStackChange;
	
protected:
	virtual void BeginPlay() override;

private:
	void ConstructInventory();

	TWeakObjectPtr<APlayerController> OwningController;

	UPROPERTY(Replicated)
	FInv_InventoryFastArray InventoryList;

	UPROPERTY()
	TObjectPtr<UInv_InventoryBaseWidget> InventoryMenuWidget;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TSubclassOf<UInv_InventoryBaseWidget> InventoryMenuWidgetClass;

	bool bInventoryMenuOpen{false};
	void OpenInventoryMenu();
	void CloseInventoryMenu();

	/* Actor Spawn 参数 */
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnAngleMin = -85.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnAngleMax = 85.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnDistanceMin = 10.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnDistanceMax = 50.f;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float RelativeSpawnElevation = 70.f;
	/* Actor Spawn 参数 */

};

