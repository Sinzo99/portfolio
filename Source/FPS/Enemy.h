// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class FPS_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
private:
	UPROPERTY(VisibleAnywhere, Category = "Enemy")
		float CurrentHp;
	UPROPERTY(VisibleAnywhere, Category = "Enemy")
		float MaxHP;
	UPROPERTY(VisibleAnywhere, Category = "Enemy")
		class ARifle* Rifle;
	UPROPERTY(VisibleAnywhere, Category = "Enemy")
		class UAnimMontage* HitMtg;
	UPROPERTY(VisibleAnywhere, Category = "Enemy")
		class UAnimMontage* AttackMtg;

	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<class ABullet> BulletClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float AttackRange = 1500.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float AttackRadius = 1500.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float SightRadius = 1500.f;
private:
	bool isDead;
	//float AttackRange = 600.f;
	//float AttackRadius = 20.f;
public:
	FORCEINLINE class ARifle* GetRifle() const { return Rifle; }
public:
	void PlayAttackMontage();
	void Fire();
	void SendDamage();
	void Die();
	UFUNCTION()
		void AnimNotify_Damage();
private:
	void DetectPlayer();
	AActor* TargetActor;
};
