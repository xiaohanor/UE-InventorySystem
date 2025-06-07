// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Inv_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InventorySystem.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/HUD/Inv_HUDWidget.h"

void AInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void AInv_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 获取增强输入子系统
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		// 添加输入映射上下文
		Subsystem->AddMappingContext(DefaultIMC, 0); // 0 表示优先级
	}

	InitializeHUD();
}

void AInv_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	// 将交互动作绑定到输入组件
	EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &AInv_PlayerController::PrimaryInteract);
}

void AInv_PlayerController::InitializeHUD()
{
	if (!IsLocalController()) return;

	HUDWidget = CreateWidget<UInv_HUDWidget>(this, HUDWidgetClass);
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToPlayerScreen();
	}
}

void AInv_PlayerController::PrimaryInteract()
{
	UE_LOG(LogInventory, Log, TEXT("Interact"));
}
