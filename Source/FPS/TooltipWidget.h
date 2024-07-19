// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemData.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TooltipWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
private:
	UPROPERTY(VisibleAnywhere, Category = "Tooltip", meta = (BindWidget))
		class UTextBlock* Name;
	UPROPERTY(VisibleAnywhere, Category = "Tooltip", meta = (BindWidget))
		class UTextBlock* SellPrice;
	UPROPERTY(VisibleAnywhere, Category = "Tooltip", meta = (BindWidget))
		class UTextBlock* Amount;
	UPROPERTY(VisibleAnywhere, Category = "Tooltip", meta = (BindWidget))
		class UTextBlock* Description;
	UPROPERTY(VisibleAnywhere, Category = "Tooltip", meta = (BindWidget))
		class UImage* Icon;
private:
	FItemData ItemData;
public:
	void SetItemData(FItemData& InItemData);
};
