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
		class UAnimMontage* GrabMontage; //���� ��Ÿ��
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		class UAnimMontage* UnGrabMontage; //�������� ��Ÿ��
public:
	bool isEquipped; //���� ����
	bool isEquipping; //���� ������ ����
public:
	void Equip(); //���� ���� �Լ�
	void BeginEquip(); //���� ���� �Լ�
	void EndEquip(); //���� ���� �Լ�

	void UnEquip(); //���� ���� �Լ�
	void BeginUnEquip(); //���� ���� ����
	void EndUnEquip(); //���� ���� ����
public:
	FORCEINLINE bool GetIsEquipped() { return isEquipped; }
	FORCEINLINE bool GetIsEquipping() { return isEquipping; }
private:
	bool isAiming; //����������
public:
	FORCEINLINE bool GetIsAiming() { return isAiming; }
	FORCEINLINE void SetIsAiming(bool bAiming) { isAiming = bAiming; }
public:
	bool isFiring; //���� ������
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
private: //����Ʈ
	UPROPERTY(VisibleDefaultsOnly, Category = "FX")
		class UParticleSystem* MuzzleParticle; //�ѱ� ����Ʈ
	UPROPERTY(VisibleDefaultsOnly, Category = "FX")
		class UParticleSystem* ImpactParticle; //Ÿ�� ����Ʈ
private: //����
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
