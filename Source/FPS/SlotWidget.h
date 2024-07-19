// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemData.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API USlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	USlotWidget(const FObjectInitializer& FObjectInitializer);
protected:
	virtual void NativeConstruct() override;
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (BindWidget))
		class UImage* Icon;
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
		class UTexture2D* NoneTexture;
	UPROPERTY(VisibleAnywhere, Category = "Inventory", meta = (BindWidget))
		class UTextBlock* Amount;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
		struct FItemData ItemData;
public:
	void SetItemData(struct FItemData& InItemData);
private:
	UPROPERTY(VisibleAnywhere, Category = "Tooltip")
		TSubclassOf<class UUserWidget> TooltipClass;
	UPROPERTY(VisibleAnywhere, Category = "Tooltip")
		class UTooltipWidget* Tooltip;
private:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, class UDragDropOperation*& OutOperation) override; //드래그 시작시 호출되는 함수

	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, class UDragDropOperation* InOperation) override; //드래그 취소시 호출

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, class UDragDropOperation* InOperation) override; //드래그 종료시 호출
public:
	void SpawnItem();
private:
	UPROPERTY()
		class AGunner* Gunner;
};
