// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "ItemData.h"
#include "SlotWidget.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	for (int i = 0; i < 23; i++)
	{
		FName SlotName = FName(FString::Printf(TEXT("WB_Slot_%d"), i));
		USlotWidget* SlotWidget = Cast<USlotWidget>(GetWidgetFromName(SlotName));
		if (SlotWidget == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Not Slot!"));
			return;
		}
		Slots.Add(SlotWidget);
	}
}

void UInventoryWidget::AddSlotToItem(FItemData& InItemData)
{
	for (USlotWidget* SlotWidget : Slots)
	{
		if (SlotWidget->ItemData == InItemData && InItemData.ItemType == EItemType::Equipment)
		{
			continue;
		}
		else if (SlotWidget->ItemData.Amount == 0)
		{
			SlotWidget->ItemData = InItemData;
			if (InItemData.Amount == 0)
			{
				SlotWidget->ItemData.Amount++;
			}
			SlotWidget->SetItemData(SlotWidget->ItemData);
			return;
		}
		else if (SlotWidget->ItemData == InItemData)
		{
			if (InItemData.Amount == 0)
			{
				SlotWidget->ItemData.Amount++;
			}
			SlotWidget->ItemData.Amount += InItemData.Amount;
			SlotWidget->SetItemData(SlotWidget->ItemData);
			return;
		}
	}
}
