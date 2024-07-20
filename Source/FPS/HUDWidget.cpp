// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"

#include "Gunner.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Gunner = Cast<AGunner>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (IsValid(Gunner))
	{
		Gunner->HUDSignature.BindUObject(this, &UHUDWidget::UpdateHUDWidget);
	}
}

void UHUDWidget::UpdateHUDWidget()
{
	HealthBar->SetPercent(Gunner->GetCurrentHp() / Gunner->GetMaxHp());

	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0); //소수점 표기 안함

	CurrentHp->SetText(FText::AsNumber(Gunner->GetCurrentHp(), &Opts));
	MaxHp->SetText(FText::AsNumber(Gunner->GetMaxHp(), &Opts));
}
