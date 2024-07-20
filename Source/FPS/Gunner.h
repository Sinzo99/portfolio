// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IRifle.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gunner.generated.h"

DECLARE_DELEGATE(FHUDSignature);

//DECLARE_DELEGATE(FAmmoSignature);


UCLASS()
class FPS_API AGunner : public ACharacter, public IIRifle
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGunner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Camera")
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, Category = "Camera")
		class UCameraComponent* Camera;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		class UInventoryComponent* InventoryComponent;
private: //입력함수
	UFUNCTION()
		void MoveForward(float Axis);
	UFUNCTION()
		void MoveRight(float Axis);
	UFUNCTION()
		void Turn(float Axis);
	UFUNCTION()
		void LookUp(float Axis);
	UFUNCTION()
		void OnRunning();
	UFUNCTION()
		void OffRunning();
	UFUNCTION()
		void OnRifle();
	UFUNCTION()
		void Reload();
	UFUNCTION()
		void Inventory();
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		class ARifle* Rifle;
public:
	FORCEINLINE class ARifle* GetRifle() override
	{
		return Rifle;
	}
private:
	UFUNCTION()
		void OnAim();
	UFUNCTION()
		void OffAim();
public:
	bool isAiming;
	bool isFiring;
	bool isInventory;
private:
	UPROPERTY(EditDefaultsOnly, Category = "CrossHair")
		TSubclassOf<class UCrossHairWidget> CrossHairClass;
	UPROPERTY(EditDefaultsOnly, Category = "CrossHair")
		class UCrossHairWidget* CrossHair;
public:
	virtual void OnFocus() override;
	virtual void OffFocus() override;
	virtual void GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection) override;
public:
	UFUNCTION()
		void OnFire();
	UFUNCTION()
		void Fire();
	UFUNCTION()
		void OffFire();
public:
	void PlayCameraShake();
private:
	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<class UUserWidget> HUDWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<class UUserWidget> AmmoWidgetClass;
public:
	UPROPERTY(VisibleAnywhere, Category = "State")
		float Damage = 10.f;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		float CurrentHp = 100.f;
	UPROPERTY(VisibleAnywhere, Category = "Player")
		float MaxHp = 100.f;
public:
	FHUDSignature HUDSignature;
	FTimerHandle Timer;
public:
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	FORCEINLINE float GetMaxHp() { return MaxHp; }
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniMap", meta = (AllowPrivateAccess = true))
		class USpringArmComponent* MinimapArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniMap", meta = (AllowPrivateAccess = true))
		class USceneCaptureComponent2D* MinimapCamera;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Minimap")
		TSubclassOf<class UUserWidget> MinimapClass;
	UPROPERTY(EditDefaultsOnly, Category = "Minimap")
		class UUserWidget* Minimap;
protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void Dead();
public:
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		class ACharacter* OwnerChacter;
public:
	void RestoreHealth(float Amount);

private:
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UUserWidget> RestartWidgetClass;

	UPROPERTY()
		class UUserWidget* RestartWidget;

	void ShowRestartWidget();

	void ReStartGame();
};
