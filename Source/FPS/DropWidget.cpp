// Fill out your copyright notice in the Description page of Project Settings.


#include "DropWidget.h"

#include "SlotWidget.h"
#include "SlotDragDropOperation.h"

bool UDropWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, class UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	USlotDragDropOperation* Operation = Cast<USlotDragDropOperation>(InOperation);

	if (Operation->WidgetRef != this)
	{
		USlotWidget* NewSlot = Cast<USlotWidget>(Operation->WidgetRef);
		NewSlot->SpawnItem();
		Operation->WidgetRef->SetVisibility(ESlateVisibility::Visible);
	}
	return false;
}