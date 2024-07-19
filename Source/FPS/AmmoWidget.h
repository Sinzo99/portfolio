// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoWidget.generated.h"

/**
 *
 */
UCLASS()
class FPS_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentAmmoTextBlock;
	UPROPERTY(meta = (BindWidget))
		class UImage* Icon;
	UPROPERTY()
		class ARifle* Rifle;
public:
	void UpdateAmmoWidget();
};
