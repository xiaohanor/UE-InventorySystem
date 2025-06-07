// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/Inv_HighlightableStaticMesh.h"

void UInv_HighlightableStaticMesh::Highlight_Implementation()
{
	SetOverlayMaterial(OutlineMaterial);
}

void UInv_HighlightableStaticMesh::UnHighlight_Implementation()
{
	SetOverlayMaterial(nullptr);
}
