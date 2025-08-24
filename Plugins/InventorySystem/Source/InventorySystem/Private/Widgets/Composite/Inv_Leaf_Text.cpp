// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Composite/Inv_Leaf_Text.h"

#include "Components/TextBlock.h"

void UInv_Leaf_Text::SetText(const FText& Text) const
{
	Text_LeafText->SetText(Text);
}
