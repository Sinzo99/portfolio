// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemData.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class FPS_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	virtual void OnConstruction(const FTransform& Transform) override;
private:
	UPROPERTY(EditAnywhere, Category = "Item")
		struct FDataTableRowHandle ItemRowHandle;
	UPROPERTY(EditAnywhere, Category = "Item")
		struct FItemData ItemData;
	UPROPERTY(EditAnywhere, Category = "Item")
		class USkeletalMeshComponent* ItemMesh;
protected:
	virtual void NotifyActorBeginOverlap(class AActor* OtherActor) override;
private:
	UPROPERTY(EditAnywhere, Category = "Item")
		class USphereComponent* Sphere;
public:
	void SetItemRowName(FName InItemName);
public:
	void SetItemData(FItemData& InItemData);
	FORCEINLINE FItemData& GetItemData()
	{
		return ItemData;
	}
};
