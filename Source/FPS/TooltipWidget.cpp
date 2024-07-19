// Fill out your copyright notice in the Description page of Project Settings.


#include "TooltipWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UTooltipWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTooltipWidget::SetItemData(FItemData& InItemData)
{
	ItemData = InItemData;
	Name->SetText(FText::FromName(ItemData.ItemName));
	SellPrice->SetText(FText::AsNumber(ItemData.SellPrice));
	Amount->SetText(FText::AsNumber(ItemData.Amount));
	Description->SetText(FText::FromString(ItemData.Description));
	Icon->SetBrushFromTexture(ItemData.Icon);
	Icon->SetBrushSize(FVector2D(70, 70));
}
