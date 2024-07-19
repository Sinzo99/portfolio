// Fill out your copyright notice in the Description page of Project Settings.
//������ ������ FPS.Build.cs�� "UMG", "Slate", "SlateCore" �߰�

#include "CrossHairWidget.h"
#include "Components/Image.h"

void UCrossHairWidget::OnFocus()
{
	//�̹��� ������ ���������� ����
	CrossHair->SetBrushTintColor(FLinearColor::Red);
}

void UCrossHairWidget::OffFocus()
{
	//�̹��� ������ �ʷϻ����� ����
	CrossHair->SetBrushTintColor(FLinearColor::Green);
}
