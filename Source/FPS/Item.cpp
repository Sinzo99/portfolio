// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "Gunner.h"
#include "InventoryComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UDataTable> DataAsset(TEXT("DataTable'/Game/DataTable/DT_Item.DT_Item'"));
	if (DataAsset.Succeeded())
	{
		ItemRowHandle.DataTable = DataAsset.Object;
		ItemRowHandle.RowName = TEXT("SmallPotion");
	}

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetCollisionProfileName(FName(TEXT("Item")));
	ItemMesh->SetRelativeScale3D(FVector(1.f));
	ItemMesh->SetSimulatePhysics(true);
	RootComponent = ItemMesh;
	Tags.Add(FName("Item"));

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->AttachToComponent(ItemMesh, FAttachmentTransformRules::KeepRelativeTransform);
	Sphere->InitSphereRadius(60.f);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ItemData = *ItemRowHandle.DataTable->FindRow<FItemData>(ItemRowHandle.RowName, "");

	if (&ItemData != nullptr && ItemMesh != nullptr)
	{
		ItemMesh->SetSkeletalMesh(ItemData.ItemMesh);
	}
}

void AItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	AGunner* User = Cast<AGunner>(OtherActor);
	if (IsValid(User))
	{
		if (User->InventoryComponent != nullptr)
		{
			User->InventoryComponent->AddItem(ItemData);
			Destroy();
		}
	}
}

void AItem::SetItemRowName(FName InItemName)
{
}

void AItem::SetItemData(FItemData& InItemData)
{
	ItemData = InItemData;
}

