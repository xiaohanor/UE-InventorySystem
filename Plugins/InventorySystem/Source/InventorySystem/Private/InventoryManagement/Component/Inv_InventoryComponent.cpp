// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Component/Inv_InventoryComponent.h"

#include "Widgets/Inventory/InventoryBase/Inv_InventoryBaseWidget.h"


// Sets default values for this component's properties
UInv_InventoryComponent::UInv_InventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UInv_InventoryComponent::ToggleInventoryMenu()
{
	if (bInventoryMenuOpen)
	{
		CloseInventoryMenu();
	}
	else
	{
		OpenInventoryMenu();
	}
}


void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ConstructInventory();
}

void UInv_InventoryComponent::ConstructInventory()
{
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("库存组件需要一个玩家控制器作为拥有者"));
	if (!OwningController->IsLocalController()) return;

	InventoryMenuWidget = CreateWidget<UInv_InventoryBaseWidget>(OwningController.Get(), InventoryMenuWidgetClass);
	if (IsValid(InventoryMenuWidget))
	{
		InventoryMenuWidget->AddToPlayerScreen();
		CloseInventoryMenu();
	}
}

void UInv_InventoryComponent::OpenInventoryMenu()
{
	if (!IsValid(InventoryMenuWidget)) return;

	InventoryMenuWidget->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;

	if (!OwningController.IsValid()) return;

	FInputModeGameAndUI InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(true);
}

void UInv_InventoryComponent::CloseInventoryMenu()
{
	if (!IsValid(InventoryMenuWidget)) return;

	InventoryMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryMenuOpen = false;

	if (!OwningController.IsValid()) return;

	FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(false);
}



