// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrossHairWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UCrossHairWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CrossHair", meta = (BindWidget))
		class UImage* CrossHair;
public:
	void OnFocus();
	void OffFocus();
};
