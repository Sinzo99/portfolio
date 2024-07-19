// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Damage.h"
#include "Gunner.h"
#include "Enemy.h"

FString UAnimNotify_Damage::GetNotifyName_Implementation() const
{
	return TEXT("Damage");
}
void UAnimNotify_Damage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if (IsValid(MeshComp) == false) return;
	AEnemy* Enemy = Cast<AEnemy>(MeshComp->GetOwner());
	if (IsValid(Enemy) == false) return;
	Enemy->SendDamage();
}
