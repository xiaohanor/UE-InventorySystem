// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inv_PlayerController.generated.h"

class UInv_InventoryComponent;
class UInv_HUDWidget;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API AInv_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AInv_PlayerController();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Blueprintable)
	void ToggleInventoryMenu();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;

private:
	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;
	
	// 增强输入 //
	UPROPERTY(EditDefaultsOnly, Category="Inventory|Input")
	TObjectPtr<UInputMappingContext> DefaultIMC;

	UPROPERTY(EditDefaultsOnly, Category="Inventory|Input")
	TObjectPtr<UInputAction> PrimaryInteractAction;

	UPROPERTY(EditDefaultsOnly, Category="Inventory|Input")
	TObjectPtr<UInputAction> ToggleInventoryMenuAction;
	
	void PrimaryInteract();
	// 增强输入 //

	// HUD //
	UPROPERTY(EditDefaultsOnly, Category="Inventory|HUD")
	TSubclassOf<UInv_HUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UInv_HUDWidget> HUDWidget;

	void InitializeHUD();
	// HUD //

	// 物品追踪 //
	UPROPERTY(EditDefaultsOnly, Category = "Inventory|ItemTrace")
	double TraceLength;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory|ItemTrace")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

	TWeakObjectPtr<AActor> ThisActor;
	TWeakObjectPtr<AActor> LastActor;

	void ItemTrace();
	// 物品追踪 //

};
