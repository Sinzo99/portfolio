// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Equip.h"

#include "IRifle.h"
#include "Rifle.h"

FString UAnimNotifyState_Equip::GetNotifyName_Implementation() const
{
	return TEXT("Equip"); //노티파이 이름 지정
}

void UAnimNotifyState_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if (IsValid(MeshComp))
	{
		IIRifle* RifleInterface = Cast<IIRifle>(MeshComp->GetOwner());
		if (RifleInterface != nullptr)
		{
			RifleInterface->GetRifle()->BeginEquip();
		}
	}
}

void UAnimNotifyState_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (IsValid(MeshComp))
	{
		IIRifle* RifleInterface = Cast<IIRifle>(MeshComp->GetOwner());
		if (RifleInterface != nullptr)
		{
			RifleInterface->GetRifle()->EndEquip();
		}
	}
}
