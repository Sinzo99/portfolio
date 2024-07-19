// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class FPS_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Bullet")
		class USceneComponent* Root;
	UPROPERTY(VisibleDefaultsOnly, Category = "Bullet")
		class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleDefaultsOnly, Category = "Bullet")
		class UProjectileMovementComponent* Projectile;
	UPROPERTY(VisibleDefaultsOnly, Category = "Bullet")
		class UMaterialInstanceConstant* MatIns;
private:
	UFUNCTION()
		void OnHit(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);
};
