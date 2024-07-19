// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoWidget.h"

#include "Rifle.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

#include "Components/Image.h"
#include "Engine/Texture2D.h"

void UAmmoWidget::NativeConstruct()
{
	Super::NativeConstruct();
    if (IsValid(Rifle))
    {
        Rifle->AmmoChangedSignature.AddUObject(this, &UAmmoWidget::UpdateAmmoWidget);
    }

    // CurrentAmmoTextBlock�� ���ε�
    CurrentAmmoTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("YourTextBlockName"))); // YourTextBlockName�� UI���� ������ �ؽ�Ʈ ����� �̸��Դϴ�.
    if (!IsValid(CurrentAmmoTextBlock))
    {
        UE_LOG(LogTemp, Warning, TEXT("CurrentAmmoTextBlock is not"));
    }

}

void UAmmoWidget::UpdateAmmoWidget()
{
    FNumberFormattingOptions Opts;
    Opts.SetMaximumFractionalDigits(0); // �Ҽ��� ǥ�� ����
    // Rifle�� ��ȿ���� Ȯ��
    if (IsValid(Rifle) && IsValid(CurrentAmmoTextBlock))
    {
        FText AmmoText = FText::Format(FText::FromString("{0}/{1}"), FText::AsNumber(Rifle->GetCurrentAmmo()), FText::AsNumber(Rifle->GetMaxAmmo()));
        CurrentAmmoTextBlock->SetText(AmmoText);
    }
}
