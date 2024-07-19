// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemData.h"
#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SlotDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API USlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DragDrop")
		UUserWidget* WidgetRef;
public:
	FItemData ItemData;
};
