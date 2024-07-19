// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "InventoryWidget.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> InventoryAsset(TEXT("WidgetBlueprint'/Game/Blueprint/Widget/WB_Inventory.WB_Inventory_C'"));
	if (InventoryAsset.Succeeded())
	{
		InventoryClass = InventoryAsset.Class;
	}
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	if (InventoryClass != nullptr)
	{
		InventoryWidget = Cast<UInventoryWidget>(CreateWidget<UUserWidget>(GetWorld(), InventoryClass));
		if (InventoryWidget != nullptr)
		{
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddItem(FItemData& InItem)
{
	int index = FindStack(InItem);

	InventoryWidget->AddSlotToItem(InItem);

	if (index == -1)
	{
		FItemData NewItemData = InItem;
		NewItemData.Amount = 1;
		ItemDatas.Add(NewItemData);
	}
	else
	{
		ItemDatas[index].Amount++;
	}
}

void UInventoryComponent::ShowInventroy()
{
	if (IsValid(InventoryWidget) == false) return;
	if (isShowInventory == false)
	{
		isShowInventory = true;
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController->SetShowMouseCursor(true);
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
		InputMode.SetHideCursorDuringCapture(false);
		PlayerController->SetInputMode(FInputModeGameAndUI(InputMode));
	}
	else
	{
		isShowInventory = false;
		auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController->SetShowMouseCursor(false);
		PlayerController->SetInputMode(FInputModeGameOnly());
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

int UInventoryComponent::FindStack(FItemData& InItemData)
{
	for (int i = 0; i < ItemDatas.Num(); i++)
	{
		if (ItemDatas[i] == InItemData)
		{
			return i;
		}
	}
	return -1;
}

TArray<FItemData> UInventoryComponent::GetItemDatas()
{
	return ItemDatas;
}

