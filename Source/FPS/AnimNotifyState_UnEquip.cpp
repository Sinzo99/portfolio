// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_UnEquip.h"

#include "IRifle.h"
#include "Rifle.h"

FString UAnimNotifyState_UnEquip::GetNotifyName_Implementation() const
{
	return TEXT("UnEquip");
}

void UAnimNotifyState_UnEquip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if (IsValid(MeshComp))
	{
		IIRifle* RifleInterface = Cast<IIRifle>(MeshComp->GetOwner());
		if (RifleInterface != nullptr)
		{
			RifleInterface->GetRifle()->BeginUnEquip();
		}
	}
}

void UAnimNotifyState_UnEquip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
		if (IsValid(MeshComp))
		{
			IIRifle* RifleInterface = Cast<IIRifle>(MeshComp->GetOwner());
			if (RifleInterface != nullptr)
			{
				RifleInterface->GetRifle()->EndUnEquip();
			}
		}
}
