// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Materials/MaterialInstanceConstant.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));

	// * ��Ƽ����
	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant>MaterialAsset(TEXT("MaterialInstanceConstant'/Game/Material/MAT_Bullet_Inst.MAT_Bullet_Inst'"));
	if (MaterialAsset.Succeeded())
	{
		MatIns = MaterialAsset.Object;
	}

	// * �޽�
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
	}

	Mesh->SetMaterial(0, MatIns);
	Mesh->SetRelativeScale3D(FVector(1, 0.025f, 0.025f));
	Mesh->SetupAttachment(Root);

	Projectile->InitialSpeed = 40000.f; //4��
	Projectile->MaxSpeed = 40000.f; //4��
	Projectile->ProjectileGravityScale = 0.f;
	Projectile->SetUpdatedComponent(Root);
	InitialLifeSpan = 10.f; //10�� �� ���� ����
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	Destroy();
}

