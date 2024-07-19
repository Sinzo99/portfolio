// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemData.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	void AddItem(struct FItemData& InItem);
	void ShowInventroy();
	int FindStack(struct FItemData& InItemData);
private:
	UPROPERTY(VisibleAnywhere, Category = "Item")
		TArray<FItemData> ItemDatas;
private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = true))
		TSubclassOf<class UUserWidget> InventoryClass;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = true))
		class UInventoryWidget* InventoryWidget;
public:
	bool isShowInventory = false;
public:
	UFUNCTION()
		TArray<FItemData> GetItemDatas();
		
};
