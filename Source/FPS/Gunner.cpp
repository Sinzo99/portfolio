// Fill out your copyright notice in the Description page of Project Settings.


#include "Gunner.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CrossHairWidget.h"
#include "Rifle.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "FPSCameraShake.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "AmmoWidget.h"
#include "GunnerAnimInstance.h"

AGunner::AGunner()
{
	PrimaryActorTick.bCanEverTick = true;

	// * ī�޶�
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// * �޽�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/Man/Mesh/Full/SK_Man_Full_04.SK_Man_Full_04'"));

	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	Tags.Add(FName(TEXT("Player")));

	// * ī�޶� ��ġ
	SpringArm->SetRelativeLocation(FVector(0, 0, 60));
	SpringArm->TargetArmLength = 200.f;
	SpringArm->bDoCollisionTest = true;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0, 30, 0);

	// * �ִϸ��̼�
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("AnimBlueprint'/Game/Blueprint/Gunner/ABP_Gunner.ABP_Gunner_C'"));
	if (AnimAsset.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimAsset.Class);
	}

	// * ũ�ν����
	static ConstructorHelpers::FClassFinder<UUserWidget> CrossHairAsset(TEXT("WidgetBlueprint'/Game/Blueprint/Widget/WB_CrossHair.WB_CrossHair_C'"));
	if (CrossHairAsset.Succeeded())
	{
		CrossHairClass = CrossHairAsset.Class;
	}

	// * �̴ϸ�
	MinimapArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapArm"));
	MinimapArm->SetupAttachment(GetMesh());
	MinimapArm->SetRelativeLocation(FVector(0, 0, 90.f));
	MinimapArm->SetRelativeRotation(FRotator(-90, 90, 0));
	MinimapArm->bDoCollisionTest = false;
	MinimapArm->TargetArmLength = 700.f;

	MinimapCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapCamera"));
	MinimapCamera->SetupAttachment(MinimapArm);
	MinimapCamera->SetRelativeLocation(FVector::ZeroVector);
	MinimapCamera->SetRelativeRotation(FRotator::ZeroRotator);

	static ConstructorHelpers::FObjectFinder<UCanvasRenderTarget2D> MinimapAsset(TEXT("CanvasRenderTarget2D'/Game/Blueprint/Minimap/CRT_Minimap.CRT_Minimap'"));
	if (MinimapAsset.Succeeded())
	{
		MinimapCamera->TextureTarget = MinimapAsset.Object;
	}

	// * HUD����
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetAsset(TEXT("WidgetBlueprint'/Game/Blueprint/Widget/WB_HUDWidget.WB_HUDWidget_C'"));
	if (WidgetAsset.Succeeded())
	{
		HUDWidgetClass = WidgetAsset.Class;
	}

	// * �̴ϸ� ����
	static ConstructorHelpers::FClassFinder<UUserWidget> MinimapWidgetAsset(TEXT("WidgetBlueprint'/Game/Blueprint/Minimap/WB_Minimap.WB_Minimap_C'"));
	if (MinimapWidgetAsset.Succeeded())
	{
		MinimapClass = MinimapWidgetAsset.Class;
	}

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	static ConstructorHelpers::FClassFinder<UUserWidget> RestartWidgetAsset(TEXT("WidgetBlueprint'/Game/Blueprint/Widget/WB_Restart.WB_Restart_C'"));
	if (RestartWidgetAsset.Succeeded())
	{
		RestartWidgetClass = RestartWidgetAsset.Class;
	}
}

void AGunner::BeginPlay()
{
	Super::BeginPlay();
	//������ ����
	Rifle = ARifle::Spawn(GetWorld(), this);

	CrossHair = CreateWidget<UCrossHairWidget, APlayerController>(GetController<APlayerController>(), CrossHairClass); //���� ����
	CrossHair->AddToViewport(); //����Ʈ �߰�
	CrossHair->OffFocus();

	// * �̴ϸ�
	Minimap = CreateWidget<UUserWidget>(GetWorld(), MinimapClass);
	Minimap->AddToViewport();

	// * HUD����
	if (HUDWidgetClass != nullptr)
	{
		CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass)->AddToViewport();
	}

	CurrentHp = MaxHp;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController && PlayerController->GetPawn() != this)
	{
		PlayerController->Possess(this);
	}

	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}

	/*if (AmmoWidgetClass != nullptr)
	{
		CreateWidget<UUserWidget>(GetWorld(), AmmoWidgetClass)->AddToViewport();
	}*/
}

// Called every frame
void AGunner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isAiming && SpringArm->TargetArmLength > 100.f)
	{
		SpringArm->TargetArmLength -= 5;
	}
	else if (isAiming == false && SpringArm->TargetArmLength < 200.f)
	{
		SpringArm->TargetArmLength += 5;
	}
	isInventory = false;
}

// Called to bind functionality to input
void AGunner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AGunner::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AGunner::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AGunner::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AGunner::LookUp);

	// * �޸���
	PlayerInputComponent->BindAction(TEXT("Running"), IE_Pressed, this, &AGunner::OnRunning);
	PlayerInputComponent->BindAction(TEXT("Running"), IE_Released, this, &AGunner::OffRunning);

	// * ����
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AGunner::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AGunner::StopJumping);

	// * �� ����
	PlayerInputComponent->BindAction(TEXT("Rifle"), IE_Pressed, this, &AGunner::OnRifle);

	// * �κ��丮
	PlayerInputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &AGunner::Inventory);

	// * ����
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &AGunner::OnAim);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &AGunner::OffAim);

	// * ����
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AGunner::OnFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AGunner::OffFire);

	// * ������
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AGunner::Reload);

}

void AGunner::MoveForward(float Axis)
{
	FRotator YawRot = FRotator(0, GetControlRotation().Yaw, 0);
	FVector Direction = FQuat(YawRot).GetForwardVector().GetUnsafeNormal2D();
	AddMovementInput(Direction, Axis);
}

void AGunner::MoveRight(float Axis)
{
	FRotator YawRot = FRotator(0, GetControlRotation().Yaw, 0);
	FVector Direction = FQuat(YawRot).GetRightVector().GetUnsafeNormal2D();
	AddMovementInput(Direction, Axis);
}

void AGunner::Turn(float Axis)
{
	AddControllerYawInput(Axis);
}

void AGunner::LookUp(float Axis)
{
	AddControllerPitchInput(Axis);
}

void AGunner::OnRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 400;
}

void AGunner::OffRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 300;
}

void AGunner::OnRifle()
{
	if (Rifle->GetIsEquipped())
	{
		Rifle->UnEquip();

		return;
	}
	Rifle->Equip();
}

void AGunner::Reload()
{
	Rifle->BeginReload();
}

void AGunner::Inventory()
{
	InventoryComponent->ShowInventroy();
}

void AGunner::OnAim()
{
	if (Rifle->GetIsEquipped() && !Rifle->GetIsEquipping() && !InventoryComponent->isShowInventory && !GetCharacterMovement()->IsFalling())
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		isAiming = true;
		SpringArm->SocketOffset = FVector(0, 30, 10);
		Camera->FieldOfView = 60; //�þ߰�
		Rifle->SetIsAiming(isAiming);
		CrossHair->OffFocus();
		//CrossHair->SetVisibility(ESlateVisibility::Visible);
	}
}

void AGunner::OffAim()
{
	if (Rifle->GetIsEquipped() && !Rifle->GetIsEquipping())
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		isAiming = false;
		SpringArm->SocketOffset = FVector(0, 30, 0);
		Camera->FieldOfView = 90; //�þ߰�
		Rifle->SetIsAiming(isAiming);
		CrossHair->OffFocus();
		//CrossHair->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AGunner::OnFocus()
{
	CrossHair->OnFocus();
}

void AGunner::OffFocus()
{
	CrossHair->OffFocus();
}

void AGunner::GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection)
{
	OutDirection = Camera->GetForwardVector(); //ī�޶� �չ���
	FTransform Transform = Camera->GetComponentToWorld(); //ī�޶� ���� Ʈ������
	FVector CameraLocation = Transform.GetLocation(); //ī�޶� ��ġ
	OutStart = CameraLocation + OutDirection;

	//������ ������ ������ ���Գ��� ���̰� 1�� ����
	FVector ConeDirection = UKismetMathLibrary::RandomUnitVectorInEllipticalConeInDegrees(OutDirection, 0.2f, 0.3f);
	ConeDirection *= 3000.f;

	OutEnd = CameraLocation + ConeDirection;
}

void AGunner::OnFire()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		Fire();
	}
}

void AGunner::Fire()
{
	Rifle->BeginFire();
}

void AGunner::OffFire()
{
	Rifle->EndFire();
}

void AGunner::PlayCameraShake()
{
	auto CameraManager = GetController<APlayerController>()->PlayerCameraManager;
	if (IsValid(CameraManager))
	{
		//ī�޶� ���
		CameraManager->PlayCameraShake(UFPSCameraShake::StaticClass());
	}
}

float AGunner::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentHp -= DamageAmount;
	if (HUDSignature.IsBound())
	{
		HUDSignature.Execute();
	}
	if (CurrentHp <= 0)
	{
		Destroy();
		Dead();
	}
	return DamageAmount;
}

void AGunner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (HUDSignature.IsBound())
	{
		HUDSignature.Unbind();
	}
}

void AGunner::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorEnableCollision(false);
	GetCharacterMovement()->DisableMovement();

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetPhysicsBlendWeight(1.f);

	ShowRestartWidget();
}

void AGunner::RestoreHealth(float Amount)
{
	CurrentHp += Amount;
	if (CurrentHp > MaxHp)
	{
		CurrentHp = MaxHp;
	}
	if (HUDSignature.IsBound())
	{
		HUDSignature.Execute();
	}
}

void AGunner::ShowRestartWidget()
{
	if (RestartWidgetClass)
	{
		RestartWidget = CreateWidget<UUserWidget>(GetWorld(), RestartWidgetClass);

		if (RestartWidget)
		{
			RestartWidget->AddToViewport();

			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
			if (PlayerController)
			{
				PlayerController->bShowMouseCursor = true;
				FInputModeUIOnly InputModeData;
				InputModeData.SetWidgetToFocus(RestartWidget->TakeWidget());
				PlayerController->SetInputMode(InputModeData);
			}
		}
	}
}

void AGunner::ReStartGame()
{
	if (RestartWidget)
	{
		RestartWidget->RemoveFromViewport();
		RestartWidget = nullptr;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetActorEnableCollision(true);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetAllBodiesSimulatePhysics(false);
	GetMesh()->SetPhysicsBlendWeight(0.f);

	CurrentHp = MaxHp;

	if (PlayerController && PlayerController->GetPawn() != this)
	{
		PlayerController->Possess(this);
	}

	FName CurrentLevelName = FName(*GetWorld()->GetName());
	UGameplayStatics::OpenLevel(this, CurrentLevelName, false);
}

