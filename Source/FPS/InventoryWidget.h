// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory")
		TArray<class USlotWidget*> Slots;
public:
	void AddSlotToItem(struct FItemData& InItemData);
};
