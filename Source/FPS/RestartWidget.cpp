// Fill out your copyright notice in the Description page of Project Settings.


#include "RestartWidget.h"
#include "Components/Button.h"
#include "Kismet/GamePlayStatics.h"
#include "GameFramework/PlayerController.h"
#include "FPSGameModeBase.h"

void URestartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind the restart button click event
	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &URestartWidget::OnRestartButtonClicked);
	}

	// Bind the quit button click event
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &URestartWidget::OnQuitButtonClicked);
	}
}
void URestartWidget::OnRestartButtonClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (AFPSGameModeBase* GameMode = Cast<AFPSGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->RestartGame();
	}
}

void URestartWidget::OnQuitButtonClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		PlayerController->ConsoleCommand("Quit");
	}
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
