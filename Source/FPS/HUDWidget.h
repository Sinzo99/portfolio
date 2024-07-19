// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
private:
	UPROPERTY(VisibleAnyWhere, meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(VisibleAnyWhere, meta = (BindWidget))
		class UTextBlock* CurrentHp;
	UPROPERTY(VisibleAnyWhere, meta = (BindWidget))
		class UTextBlock* MaxHp;
	UPROPERTY()
		class AGunner* Gunner;
public:
	void UpdateHUDWidget();
};
