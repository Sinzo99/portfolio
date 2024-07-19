// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Bullet.h"
#include "Rifle.h"
#include "IRifle.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "Materials/MaterialInstanceConstant.h"
#include "DrawDebugHelpers.h"
#include "Gunner.h"
#include "FPSGameModeBase.h"

// Sets default values
AEnemy::AEnemy() : CurrentHp(100), MaxHP(100), TargetActor(nullptr)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// * 메쉬
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/Man/Mesh/Full/SK_Man_Full_04.SK_Man_Full_04'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -88.f));
		GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));
	}

	// * 애니메이션
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("AnimBlueprint'/Game/Blueprint/Enemy/ABP_Enemy.ABP_Enemy_C'"));
	if (AnimAsset.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
	}
	// * 히트 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMtgAsset(TEXT("AnimMontage'/Game/Blueprint/Enemy/MTG_EnemyHit.MTG_EnemyHit'"));
	if (HitMtgAsset.Succeeded())
	{
		HitMtg = HitMtgAsset.Object;
	}

	// * 공격 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMtgAsset(TEXT("AnimMontage'/Game/Blueprint/Enemy/MTG_EnemyFire.MTG_EnemyFire'"));
	if (AttackMtgAsset.Succeeded())
	{
		AttackMtg = AttackMtgAsset.Object;
	}

	// * 인공지능
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	Tags.Add(FName(TEXT("Enemy")));

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	Rifle = ARifle::Spawn(GetWorld(), this);
	Rifle->BeginEquip();
	CurrentHp = MaxHP;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DetectPlayer();

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	CurrentHp -= Damage;
	PlayAnimMontage(HitMtg, 1.2f);
	if (CurrentHp <= 0)
	{
		Rifle->Destroy();
		Die();
	}
	return Damage;
}

void AEnemy::PlayAttackMontage()
{
	PlayAnimMontage(AttackMtg);
	Rifle->BeginFire();
	Fire();
}

void AEnemy::Fire()
{
	if (BulletClass)
	{
		FVector MuzzleLocation = Rifle->GetActorLocation() + FTransform(Rifle->GetActorRotation()).TransformVector(FVector(100.0f, 0.0f, 10.0f)); // Adjust as necessary
		FRotator MuzzleRotation = Rifle->GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		if (Bullet)
		{
			FVector LaunchDirection = MuzzleRotation.Vector();
		}
	}
}

void AEnemy::SendDamage()
{
	FVector Start, End, Direction;
	Start = GetActorLocation();
	Direction = GetActorForwardVector();
	End = Start + (Direction * AttackRange);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TEnumAsByte<EObjectTypeQuery> PawnType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(PawnType);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	FHitResult HitResult;
	float Damage = 10.f;

	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActors(IgnoreActors);
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
	if (GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, FCollisionObjectQueryParams(ObjectTypes), CollisionParams))
	{
		if (IsValid(HitResult.GetActor()))
		{
			if (HitResult.GetActor()->ActorHasTag(FName(TEXT("Player"))))
			{
				UGameplayStatics::ApplyDamage(HitResult.GetActor(), Damage, GetController(), this, UDamageType::StaticClass());
			}
		}
		
	}
	if (TargetActor)
	{
		UGameplayStatics::ApplyDamage(TargetActor, Damage, GetController(), this, UDamageType::StaticClass());
	}
}

void AEnemy::Die()
{
	if (isDead)
	{
		return;
	}
	isDead = true;

	AFPSGameModeBase* GameMode = Cast<AFPSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->DecrementEnemyCount();
	}
	Destroy();
}

void AEnemy::AnimNotify_Damage()
{
	SendDamage();
}

void AEnemy::DetectPlayer()
{
	FVector Start = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector End = Start + ForwardVector * SightRadius;
	float Damage = 10.0f;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActors(IgnoreActors);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);

	if (GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, FCollisionObjectQueryParams(ObjectTypes), CollisionParams))
	{
		if (IsValid(HitResult.GetActor()))
		{
			if (HitResult.GetActor()->ActorHasTag(FName(TEXT("Player"))))
			{
				TargetActor = HitResult.GetActor();

				PlayAttackMontage();
			}
		}
		else
		{
			TargetActor = nullptr;
		}
	}
}
