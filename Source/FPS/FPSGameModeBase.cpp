// Copyright Epic Games, Inc. All Rights Reserved.


#include "FPSGameModeBase.h"

#include "Gunner.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Enemy.h"
#include "GameClearWidget.h"

AFPSGameModeBase::AFPSGameModeBase()
{
	DefaultPawnClass = AGunner::StaticClass();
	TotalEnemies = 0;
	CurrentEnemies = 0;
    if (RestartWidgetClass == nullptr)
    {
        static ConstructorHelpers::FClassFinder<UUserWidget> RestartWidgetAsset(TEXT("WidgetBlueprint'/Game/Blueprint/Widget/WB_GameClear.WB_GameClear_C'"));
        if (RestartWidgetAsset.Succeeded())
        {
            RestartWidgetClass = RestartWidgetAsset.Class;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to find WB_Restart widget class."));
        }
    }
}

void AFPSGameModeBase::DecrementEnemyCount()
{
    CurrentEnemies--;
    CheckForGameClear();
}

void AFPSGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> Enemy;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Enemy);
    TotalEnemies = Enemy.Num();
    CurrentEnemies = TotalEnemies;
}

void AFPSGameModeBase::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AFPSGameModeBase::CheckForGameClear()
{
    if (CurrentEnemies <= 0)
    {
        ShowRestartWidget();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Game not clear yet. Remaining enemies: %d"), CurrentEnemies); // 콘솔에 남은 적의 수를 로그로 출력
    }
}

void AFPSGameModeBase::ShowRestartWidget()
{
    if (RestartWidgetClass)
    {
        RestartWidget = CreateWidget<UUserWidget>(GetWorld(), RestartWidgetClass);

        if (RestartWidget)
        {
            RestartWidget->AddToViewport();

            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
            if (PlayerController)
            {
                PlayerController->bShowMouseCursor = true;
                FInputModeUIOnly InputModeData;
                InputModeData.SetWidgetToFocus(RestartWidget->TakeWidget());
                PlayerController->SetInputMode(InputModeData);
            }
        }
    }
}
