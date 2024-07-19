// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "IRifle.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Gunner.h"
#include "Particles/ParticleSystem.h"

#include "Sound/SoundCue.h"
#include "Bullet.h"

#include "Materials/MaterialInstanceConstant.h"

#include "Blueprint/UserWidget.h"
#include "AmmoWidget.h"

#include "Enemy.h"

#include "Components/TextBlock.h"

#include "Components/Image.h"
#include "Engine/Texture2D.h"

ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RifleMesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'"));
	if (MeshAsset.Succeeded())
	{
		Mesh->SetSkeletalMesh(MeshAsset.Object);
	}
	RootComponent = Mesh;

	// * √— ¿Â¬¯, «ÿ¡¶ ∏˘≈∏¡÷
	static ConstructorHelpers::FObjectFinder<UAnimMontage> GrabMtgAsset(TEXT("AnimMontage'/Game/Blueprint/Gunner/MTG_EquipRifle.MTG_EquipRifle'"));
	if (GrabMtgAsset.Succeeded())
	{
		GrabMontage = GrabMtgAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UnGrabMtgAsset(TEXT("AnimMontage'/Game/Blueprint/Gunner/MTG_UnEquipRifle.MTG_UnEquipRifle'"));
	if (UnGrabMtgAsset.Succeeded())
	{
		UnGrabMontage = UnGrabMtgAsset.Object;
	}

	// * ∞¯∞› ∏˘≈∏¡÷
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireMtgAsset(TEXT("AnimMontage'/Game/Blueprint/Gunner/MTG_Fire.MTG_Fire'"));
	if (FireMtgAsset.Succeeded())
	{
		FireMontage = FireMtgAsset.Object;
	}

	// * ¿Á¿Â¿¸ ∏˘≈∏¡÷
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadMtgAsset(TEXT("AnimMontage'/Game/Blueprint/Gunner/MTG_Reload.MTG_Reload'"));
	if (ReloadMtgAsset.Succeeded())
	{
		ReloadMontage = ReloadMtgAsset.Object;
	}

	// * ∏”¡Ò¿Ã∆Â∆Æ
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MuzzleEffectAsset(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_FireBall_Powerup.P_FireBall_Powerup'"));
	if (MuzzleEffectAsset.Succeeded())
	{
		MuzzleParticle = MuzzleEffectAsset.Object;
	}

	// * ≈∏∞›¿Ã∆Â∆Æ
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ImpactEffectAsset(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Impacts/P_Impact_Shield_Fire.P_Impact_Shield_Fire'"));
	if (ImpactEffectAsset.Succeeded())
	{
		ImpactParticle = ImpactEffectAsset.Object;
	}

	// * ªÁøÓµÂ
	static ConstructorHelpers::FObjectFinder<USoundCue> FireSoundAsset(TEXT("SoundCue'/Game/FPSAsset/ak_47_Sound_Cue.ak_47_Sound_Cue'"));
	if (FireSoundAsset.Succeeded())
	{
		FireSoundCue = FireSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> ReloadSoundAsset(TEXT("SoundCue'/Game/FPSAsset/Reload_Sound_Cue.Reload_Sound_Cue'"));
	if (ReloadSoundAsset.Succeeded())
	{
		ReloadSoundCue = ReloadSoundAsset.Object;
	}
	// * µ•ƒÆ ∏”∆º∏ÆæÛ
	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant>BulletMarkMatAsset(TEXT("MaterialInstanceConstant'/Game/Material/MAT_BulletMark_Inst.MAT_BulletMark_Inst'"));
	if (BulletMarkMatAsset.Succeeded())
	{
		BulletMarkMat = BulletMarkMatAsset.Object;
	}

	//* Ammo ¿ß¡¨
	static ConstructorHelpers::FClassFinder<UUserWidget> AmmoWidgetAsset(TEXT("WidgetBlueprint'/Game/Blueprint/Widget/WB_Ammo.WB_Ammo_C'"));
	if (AmmoWidgetAsset.Succeeded())
	{
		AmmoWidgetClass = AmmoWidgetAsset.Class;
	}
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();
	OwnerChacter = Cast<ACharacter>(GetOwner());
	SetActorHiddenInGame(false);

	if (AmmoWidgetClass != nullptr)
	{
		UUserWidget* AmmoWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), AmmoWidgetClass);
		if (AmmoWidgetInstance != nullptr)
		{
			AmmoWidgetInstance->AddToViewport();
			CurrentAmmoTextBlock = Cast<UTextBlock>(AmmoWidgetInstance->GetWidgetFromName(TEXT("CurrentAmmoTextBlock")));
		}
	}
}

// Called every frame
void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!isAiming) return;
	IIRifle* RifleInterface = Cast<IIRifle>(OwnerChacter);
	if (RifleInterface == nullptr) return;
	FVector Start, End, Direction;
	RifleInterface->GetLocationAndDirection(Start, End, Direction);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OwnerChacter);
	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Pawn, Params))
	{
		ACharacter* Other = Cast<ACharacter>(HitResult.GetActor());
		if (IsValid(Other) == false) return;
		if (Other->ActorHasTag(FName(TEXT("Enemy"))))
		{
			RifleInterface->OnFocus();
			isTarget = true;
			Target = Other;
			return;
		}
	}
	isTarget = false;
	RifleInterface->OffFocus();
	Target = nullptr;
	isReloading = false;
	CurrentAmmoTextBlock->SetText(FText::AsNumber(GetCurrentAmmo()));
}

//∆—≈‰∏Æ ∆–≈œ
ARifle* ARifle::Spawn(UWorld* InWorld, ACharacter* InOwner)
{
	FActorSpawnParameters Params;
	Params.Owner = InOwner;
	return InWorld->SpawnActor<ARifle>(Params);
}

void ARifle::Equip()
{
	//√— ¿Â¬¯«œ¡ˆ æ æ“∞Ì ¿Â¬¯¡ﬂ¿Ã æ∆¥œ∂Û∏È
	if (isEquipped == false && isEquipping == false)
	{
		isEquipping = true;
		OwnerChacter->PlayAnimMontage(GrabMontage); //¿Â¬¯∏˘≈∏¡÷ ¿Áª˝
		CurrentAmmoTextBlock->SetText(FText::AsNumber(GetCurrentAmmo()));
	}
}

void ARifle::BeginEquip()
{
	isEquipped = true; //¿Â¬¯ «ﬂ¿Ω
	SetActorHiddenInGame(false);
	AttachToComponent(OwnerChacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HandSocket); //º’ø° ∫Œ¬¯
}

void ARifle::EndEquip()
{
	isEquipping = false; //¿Â¬¯¡ﬂ æ∆¥‘
}

void ARifle::UnEquip()
{
	//¿Â¬¯«ﬂ∞Ì, ¿Â¬¯¡ﬂ¿Ã æ∆¥œ∂Û∏È
	if (isEquipped == true && isEquipping == false)
	{
		isEquipping = true; //¿Â¬¯¡ﬂ
		OwnerChacter->PlayAnimMontage(UnGrabMontage); //∏˘≈∏¡÷ ¿Áª˝
		CurrentAmmoTextBlock->SetText(FText::FromString(TEXT("")));
	}
}

void ARifle::BeginUnEquip()
{
	isEquipped = false; //¿Â¬¯ «ÿ¡¶
	AttachToComponent(OwnerChacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocket); //µÓø° ∫Œ¬¯
	SetActorHiddenInGame(true);
}

void ARifle::EndUnEquip()
{
	isEquipping = false; //¿Â¬¯¡ﬂ æ∆¥‘
}

void ARifle::BeginFire()
{
	if (isEquipped && !isEquipping && isAiming && !isFiring)
	{
		isFiring = true;
		Firing();
	}

}

void ARifle::Firing()
{
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;
		
		OwnerChacter->PlayAnimMontage(FireMontage);

		AGunner* Gunner = Cast<AGunner>(OwnerChacter);
		if (IsValid(Gunner) == false) return;
		Gunner->PlayCameraShake();

		FVector MuzzleLocation = Mesh->GetSocketLocation(FName(TEXT("Muzzle")));
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSoundCue, MuzzleLocation);

		FVector Start, End, Direction;
		IIRifle* RifleInterface = Cast<IIRifle>(OwnerChacter);
		RifleInterface->GetLocationAndDirection(Start, End, Direction);
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(OwnerChacter);
		FHitResult HitResult;
		UGameplayStatics::SpawnEmitterAttached(MuzzleParticle, Mesh, TEXT("Muzzle"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

		GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), MuzzleLocation, Direction.Rotation());

		if (isFiring)
		{
			GetWorld()->GetTimerManager().SetTimer(Timer, this, &ARifle::Firing, 0.1f, false);
		}

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, Params))
		{
			FRotator Rotator = HitResult.ImpactNormal.Rotation();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitResult.Location, Rotator, FVector(2));

			UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletMarkMat, FVector(5), HitResult.Location, Rotator, 10.0f);
		}

		if (isTarget && Target != nullptr)
		{
			//≈∏∞Ÿø°∞‘ µ•πÃ¡ˆ 10 ¿¸¥ﬁ
			UGameplayStatics::ApplyDamage(Target, 10.f, OwnerChacter->GetController(), OwnerChacter, UDamageType::StaticClass());
		}
		CurrentAmmoTextBlock->SetText(FText::AsNumber(GetCurrentAmmo()));
	}
}

void ARifle::EndFire()
{
	isFiring = false;
	CurrentAmmoTextBlock->SetText(FText::AsNumber(GetCurrentAmmo()));
}

void ARifle::BeginReload()
{
	if (!isReloading && isEquipped && !isEquipping && !isFiring && CurrentAmmo != MaxAmmo)
	{
		isReloading = true;
		Reload();
	}
}

void ARifle::Reload()
{
	OwnerChacter->PlayAnimMontage(ReloadMontage);
	FVector MuzzleLocation = Mesh->GetSocketLocation(FName(TEXT("Muzzle")));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadSoundCue, MuzzleLocation);
	EndReload();
}

void ARifle::EndReload()
{
	isReloading = false;
	CurrentAmmo = MaxAmmo;
	CurrentAmmoTextBlock->SetText(FText::AsNumber(GetCurrentAmmo()));
}
