// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/Inv_InfoMsgWidget.h"

#include "Components/TextBlock.h"

void UInv_InfoMsgWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Text_Message->SetText(FText::GetEmpty());
	MessageHide();
}

void UInv_InfoMsgWidget::SetMessage(const FText& Message)
{
	Text_Message->SetText(Message);

	if (!bIsMessageActive)
	{
		MessageShow();
	}
	bIsMessageActive = true;
	GetWorld()->GetTimerManager().SetTimer(MessageTimer, [this]()
	{
		bIsMessageActive = false;
		MessageHide();
	}, MessageLifetime, false);
}
