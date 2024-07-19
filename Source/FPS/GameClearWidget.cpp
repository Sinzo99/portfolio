// Fill out your copyright notice in the Description page of Project Settings.


#include "GameClearWidget.h"
#include "Components/Button.h"
#include "Kismet/GamePlayStatics.h"
#include "GameFramework/PlayerController.h"
#include "FPSGameModeBase.h"

void UGameClearWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind the restart button click event
	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UGameClearWidget::OnRestartButtonClicked);
	}

	// Bind the quit button click event
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UGameClearWidget::OnQuitButtonClicked);
	}
}
void UGameClearWidget::OnRestartButtonClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (AFPSGameModeBase* GameMode = Cast<AFPSGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->RestartGame();
	}
}

void UGameClearWidget::OnQuitButtonClicked()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		PlayerController->ConsoleCommand("Quit");
	}
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
