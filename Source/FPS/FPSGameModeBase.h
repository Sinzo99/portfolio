// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AFPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AFPSGameModeBase();

	void DecrementEnemyCount();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Game")
		void RestartGame();
private:
	int32 TotalEnemies;
	int32 CurrentEnemies;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<class UUserWidget> RestartWidgetClass;

	UPROPERTY()
		UUserWidget* RestartWidget;

	void CheckForGameClear();
	void ShowRestartWidget();
};
