// Fill out your copyright notice in the Description page of Project Settings.
//위젯을 쓸려면 FPS.Build.cs에 "UMG", "Slate", "SlateCore" 추가

#include "CrossHairWidget.h"
#include "Components/Image.h"

void UCrossHairWidget::OnFocus()
{
	//이미지 색깔을 빨간색으로 변경
	CrossHair->SetBrushTintColor(FLinearColor::Red);
}

void UCrossHairWidget::OffFocus()
{
	//이미지 색깔을 초록색으로 변경
	CrossHair->SetBrushTintColor(FLinearColor::Green);
}
