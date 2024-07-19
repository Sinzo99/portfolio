
// Fill out your copyright notice in the Description page of Project Settings.

#include "GunnerAnimInstance.h"

#include "Gunner.h"

#include "Rifle.h"
#include "IRifle.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy.h"

void UGunnerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	Gunner = Cast<AGunner>(TryGetPawnOwner());
}

void UGunnerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (AEnemy* Enemy = Cast<AEnemy>(TryGetPawnOwner()))
	{
		Speed = Enemy->GetVelocity().Size2D();
		return;
	}

	if (IsValid(Gunner) == false) return;
	Speed = Gunner->GetVelocity().Size2D();
	//³«ÇÏ ¿©ºÎ
	isAir = Gunner->GetCharacterMovement()->IsFalling();

	Direction = CalculateDirection(Gunner->GetVelocity(), Gunner->GetControlRotation());

	Pitch = Gunner->GetBaseAimRotation().Pitch;

	IIRifle* RifleInterface = Cast<IIRifle>(Gunner);
	if (RifleInterface != nullptr)
	{
		isEquipped = RifleInterface->GetRifle()->GetIsEquipped();
		isAiming = RifleInterface->GetRifle()->GetIsAiming();

	}

}
