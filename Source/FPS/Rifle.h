// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IRifle.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rifle.generated.h"

DECLARE_MULTICAST_DELEGATE(FAmmoChangedSignature);

UCLASS()
class FPS_API ARifle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARifle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		FName HandSocket = TEXT("Hand_Rifle");
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		FName HolsterSocket = TEXT("Holster_Rifle");
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		class USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		class ACharacter* OwnerChacter;
public:
	static ARifle* Spawn(class UWorld* InWorld, class ACharacter* InOwner);
public:
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		class UAnimMontage* GrabMontage; //장착 몽타주
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		class UAnimMontage* UnGrabMontage; //장착해제 몽타주
public:
	bool isEquipped; //장착 여부
	bool isEquipping; //장착 중인지 여부
public:
	void Equip(); //장착 시작 함수
	void BeginEquip(); //장착 시작 함수
	void EndEquip(); //장착 종료 함수

	void UnEquip(); //장착 해제 함수
	void BeginUnEquip(); //장착 해제 시작
	void EndUnEquip(); //장착 해제 종료
public:
	FORCEINLINE bool GetIsEquipped() { return isEquipped; }
	FORCEINLINE bool GetIsEquipping() { return isEquipping; }
private:
	bool isAiming; //에임중인지
public:
	FORCEINLINE bool GetIsAiming() { return isAiming; }
	FORCEINLINE void SetIsAiming(bool bAiming) { isAiming = bAiming; }
public:
	bool isFiring; //공격 중인지
public:
	void BeginFire();
	void Firing();
	void EndFire();
public:
	void BeginReload();
	void Reload();
	void EndReload();
public:
	bool isReloading;
public:
	FORCEINLINE bool GetIsReloading() { return isReloading; }
private:
	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf <class UUserWidget> AmmoWidgetClass;
public:
	int MaxAmmo = 30;
	int CurrentAmmo = 30;
public:
	FORCEINLINE int GetCurrentAmmo() { return CurrentAmmo; }
	FORCEINLINE int GetMaxAmmo() { return MaxAmmo; }
private:
	bool isTarget;

	UPROPERTY(VisibleAnywhere, Category = "Target")
		class AActor* Target;
public:
	UPROPERTY(VisibleDefaultsOnly, Category = "Montage")
		class UAnimMontage* FireMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Montage")
		class UAnimMontage* ReloadMontage;
private: //이펙트
	UPROPERTY(VisibleDefaultsOnly, Category = "FX")
		class UParticleSystem* MuzzleParticle; //총구 이펙트
	UPROPERTY(VisibleDefaultsOnly, Category = "FX")
		class UParticleSystem* ImpactParticle; //타격 이펙트
private: //사운드
	UPROPERTY(VisibleDefaultsOnly, Category = "Sound")
		class USoundCue* FireSoundCue;
	UPROPERTY(VisibleDefaultsOnly, Category = "Sound")
		class USoundCue* ReloadSoundCue;
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "BulletMark")
		class UMaterialInstanceConstant* BulletMarkMat;
public:
	FTimerHandle Timer;
	FAmmoChangedSignature AmmoChangedSignature;
	UPROPERTY()
		class AGunner* User;

private:
	UPROPERTY()
		class UTextBlock* CurrentAmmoTextBlock;
	UPROPERTY()
		class UAmmoWidget* AmmoWidget;
};
