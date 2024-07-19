// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RestartWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API URestartWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnRestartButtonClicked();

	UFUNCTION()
		void OnQuitButtonClicked();

	UPROPERTY(meta = (BindWidget))
		class UButton* RestartButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;

};
