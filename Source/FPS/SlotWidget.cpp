// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "TooltipWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "SlotDragDropOperation.h"
#include "Item.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Gunner.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"

USlotWidget::USlotWidget(const FObjectInitializer& FObjectInitializer) : Super(FObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> NoneAsset(TEXT("Texture2D'/Game/Icon/NoneIcon.NoneIcon'"));
	if (NoneAsset.Succeeded())
	{
		NoneTexture = NoneAsset.Object;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> TooltipAsset(TEXT("WidgetBlueprint'/Game/Blueprint/Widget/WB_Tooltip.WB_Tooltip_C'"));
	if (TooltipAsset.Succeeded())
	{
		TooltipClass = TooltipAsset.Class;
	}
}

void USlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ItemData.Amount == 0) //�������� ���ٸ�
	{
		//����ִ� ������ ���
		Icon->SetBrushFromTexture(NoneTexture);
	}
	else //������ �ִٸ�
	{
		//�������� ������ ���
		Icon->SetBrushFromTexture(ItemData.Icon);
		Icon->SetBrushSize(FVector2D(70, 70));
	}
	Tooltip = Cast<UTooltipWidget>(CreateWidget(GetWorld(), TooltipClass));
	Tooltip->SetItemData(ItemData);
}

void USlotWidget::SetItemData(FItemData& InItemData)
{
	ItemData = InItemData; //�޾ƿ� �����͸� ���Կ� �־��ش�.
	if (ItemData.Amount == 0) //�������� ���ٸ�
	{
		Icon->SetBrushFromTexture(NoneTexture);
		Icon->SetToolTip(nullptr);
		Amount->SetText(FText::FromString(TEXT("")));
	}
	else
	{
		Icon->SetBrushFromTexture(ItemData.Icon);
		Icon->SetBrushSize(FVector2D(70, 70));
		Tooltip->SetItemData(ItemData);
		Icon->SetToolTip(Tooltip);
		Amount->SetText(FText::AsNumber(ItemData.Amount));
	}
}

FReply USlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	AGunner* User = Cast<AGunner>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	

	FEventReply Reply;
	Reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	//���� ���콺 ��ư
	if (InMouseEvent.IsMouseButtonDown(FKey("LeftMouseButton")))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Left"));
		Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		Amount->SetText(FText::AsNumber(ItemData.Amount));
	}
	//������ ���콺 ��ư
	if (InMouseEvent.IsMouseButtonDown(FKey("RightMouseButton")))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Right"));
		if (ItemData.ItemType == EItemType::Equipment)
		{
			Tooltip->SetItemData(ItemData);
			FItemData Temp;
			ItemData = Temp;
			SetItemData(ItemData);
			Icon->SetBrushFromTexture(NoneTexture);
			Icon->SetToolTip(nullptr);
		}
		//�Һ�������̶��
		if (ItemData.ItemType == EItemType::Countable)
		{
			if (ItemData.ItemName == TEXT("SmallPotion"))
			{
				User->RestoreHealth(10.f);
			}
			if (ItemData.ItemName == TEXT("MediumPotion"))
			{
				User->RestoreHealth(50.f);
			}
			if (ItemData.ItemName == TEXT("LargePotion"))
			{
				User->RestoreHealth(100.f);
			}
			ItemData.Amount--;
			Tooltip->SetItemData(ItemData);
			
			if (ItemData.Amount <= 0)
			{
				FItemData Temp;
				ItemData = Temp;
				SetItemData(ItemData);
				Icon->SetBrushFromTexture(NoneTexture);
				Icon->SetToolTip(nullptr);
				Amount->SetText(FText::FromString(TEXT("")));
			}
			else
			{
				Amount->SetText(FText::AsNumber(ItemData.Amount));
			}
		}
	}

	return Reply.NativeReply;
}

void USlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if (ItemData.Amount == 0) return;
	USlotDragDropOperation* DragDrop = NewObject<USlotDragDropOperation>();
	SetVisibility(ESlateVisibility::HitTestInvisible);
	DragDrop->ItemData = ItemData;
	DragDrop->WidgetRef = this;
	DragDrop->DefaultDragVisual = this;
	DragDrop->Pivot = EDragPivot::MouseDown;
	OutOperation = DragDrop;
}

void USlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	SetVisibility(ESlateVisibility::Visible);
}

bool USlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	USlotDragDropOperation* DragDrop = Cast<USlotDragDropOperation>(InOperation);

	if (DragDrop->WidgetRef != this)
	{
		FItemData Temp = ItemData;
		SetItemData(DragDrop->ItemData);
		USlotWidget* NewSlot = Cast<USlotWidget>(DragDrop->WidgetRef);
		if (IsValid(NewSlot))
		{
			NewSlot->SetItemData(Temp);
		}
	}
	DragDrop->WidgetRef->SetVisibility(ESlateVisibility::Visible);
	SetVisibility(ESlateVisibility::Visible);
	return false;
}

void USlotWidget::SpawnItem()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FTransform Transform = Player->GetActorTransform();
	Transform.SetLocation(Player->GetActorLocation() + (Player->GetActorForwardVector() * 200));
	AItem* Item = GetWorld()->SpawnActorDeferred<AItem>(AItem::StaticClass(), Transform); //���͸� ���������� ���忡 �߰����� ����

	Item->SetItemRowName(ItemData.ItemName); //������ ������ �־���
	Item->FinishSpawning(Transform); //������ ���͸� ���忡 �߰�
	Item->SetItemData(ItemData);


	//������ ���� �ʱ�ȭ
	FItemData Temp;
	ItemData = Temp;
	SetItemData(ItemData);
}
