#include "Items/Fragments/Inv_ItemFragment.h"

#include "Widgets/Composite/Inv_CompositeBase.h"
#include "Widgets/Composite/Inv_Leaf_Image.h"
#include "Widgets/Composite/Inv_Leaf_LabeledValue.h"
#include "Widgets/Composite/Inv_Leaf_Text.h"

void FInv_ConsumableFragment::OnConsume(APlayerController* PC)
{
	for (auto& Modifier : ConsumeModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnConsume(PC);
	}
}

void FInv_ConsumableFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	for (const auto& Modifier : ConsumeModifiers)
	{
		const auto& ModRef = Modifier.Get();
		ModRef.Assimilate(Composite);
	}
}

void FInv_ConsumableFragment::Manifest()
{
	FInv_InventoryItemFragment::Manifest();
	for (auto& Modifier : ConsumeModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.Manifest();
	}
}

void FInv_InventoryItemFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	if (!MatchesWidgetTag(Composite)) return;
	Composite->Expand();
}

bool FInv_InventoryItemFragment::MatchesWidgetTag(const UInv_CompositeBase* Composite) const
{
	return Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag());
}

void FInv_ImageFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;
	
	UInv_Leaf_Image* Image = Cast<UInv_Leaf_Image>(Composite);
	if (!IsValid(Image)) return;

	Image->SetImage(Icon);
	Image->SetBoxSize(IconDimensions);
	Image->SetImageSize(IconDimensions);
}

void FInv_LabeledNumberFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UInv_Leaf_LabeledValue* LabeledValue = Cast<UInv_Leaf_LabeledValue>(Composite);
	if (!IsValid(LabeledValue)) return;

	LabeledValue->SetText_Label(Text_Label, bCollapseLabel);

	FNumberFormattingOptions Options;
	Options.SetMaximumFractionalDigits(MaxFractionalDigits);
	Options.SetMinimumFractionalDigits(MinFractionalDigits);
	LabeledValue->SetText_Value(FText::AsNumber(Value, &Options), bCollapseValue);
}

void FInv_LabeledNumberFragment::Manifest()
{
	FInv_InventoryItemFragment::Manifest();

	// 只在Manifest时随机化一次
	if (bRandomizeOnManifest)
	{
		Value = FMath::FRandRange(Min, Max);
	}
	bRandomizeOnManifest = false;
}

void FInv_TextFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	UInv_Leaf_Text* TextLeaf = Cast<UInv_Leaf_Text>(Composite);
	if (!IsValid(TextLeaf)) return;

	TextLeaf->SetText(FragmentText);
}

void FInv_HealthPotionFragment::OnConsume(APlayerController* PC)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Health Potion consumed! Healing by: %f"), GetValue()));
}

void FInv_ManaPotionFragment::OnConsume(APlayerController* PC)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Mana Potion consumed! Mana replenished by: %f"), GetValue()));
}

void FInv_StrengthModifier::OnEquip(APlayerController* PC)
{
	FInv_EquipModifier::OnEquip(PC);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Strength Modifier equipped! Increasing strength by: %f"), GetValue()));
}

void FInv_StrengthModifier::OnUnequip(APlayerController* PC)
{
	FInv_EquipModifier::OnUnequip(PC);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Strength Modifier unequipped! Decreasing strength by: %f"), GetValue()));
}

void FInv_EquipmentFragment::OnEquip(APlayerController* PC)
{
	if (bEquipped) return;
	bEquipped = true;
	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnEquip(PC);
	}
}

void FInv_EquipmentFragment::OnUnequip(APlayerController* PC)
{
	if (!bEquipped) return;
	bEquipped = false;
	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnUnequip(PC);
	}
}

void FInv_EquipmentFragment::Assimilate(UInv_CompositeBase* Composite) const
{
	FInv_InventoryItemFragment::Assimilate(Composite);

	for (const auto& Modifier : EquipModifiers)
	{
		const auto& ModRef = Modifier.Get();
		ModRef.Assimilate(Composite);
	}
}

void FInv_EquipmentFragment::Manifest()
{
	FInv_InventoryItemFragment::Manifest();
	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.Manifest();
	}
}
