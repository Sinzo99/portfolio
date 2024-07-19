// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Reload.h"

#include "IRifle.h"
#include "Rifle.h"

FString UAnimNotifyState_Reload::GetNotifyName_Implementation() const
{
	return TEXT("Reload"); //노티파이 이름 지정
}

void UAnimNotifyState_Reload::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if (IsValid(MeshComp))
	{
		IIRifle* RifleInterface = Cast<IIRifle>(MeshComp->GetOwner());
		if (RifleInterface != nullptr)
		{
			RifleInterface->GetRifle()->BeginReload();
		}
	}
}

void UAnimNotifyState_Reload::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (IsValid(MeshComp))
	{
		IIRifle* RifleInterface = Cast<IIRifle>(MeshComp->GetOwner());
		if (RifleInterface != nullptr)
		{
			RifleInterface->GetRifle()->EndReload();
		}
	}
}
