// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

UENUM(BlueprintType, meta = (ToopTip = "ItemType"))
enum class EItemType : uint8
{
	None UMETA(DisplayName = "None"),
	Equipment UMETA(DisplayName = "Equipment"), //���
	Countable UMETA(DisplayName = "Countable"), //�Һ�
	Etc UMETA(DisplayName = "Etc") //��Ÿ
};

USTRUCT(Atomic, BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemData")
		FName ItemName = FName(TEXT("")); //�̸�
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemData")
		EItemType ItemType = EItemType::None; //Ÿ��
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemData")
		int32 BuyPrice = 0; //����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemData")
		int32 SellPrice = 0; //����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemData")
		int32 Amount = 0; //����
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemData")
		class USkeletalMesh* ItemMesh; //�޽�
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemData")
		class UTexture2D* Icon; //������
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemData")
		FString Description = FString(TEXT("")); //����

	bool operator ==(const FItemData& InItemData) const
	{
		bool isName = (ItemName == InItemData.ItemName);
		bool isType = (ItemType == InItemData.ItemType);
		bool isBuyPrice = (BuyPrice == InItemData.BuyPrice);
		bool isSellPrice = (SellPrice == InItemData.SellPrice);
		bool isMesh = (ItemMesh == InItemData.ItemMesh);
		bool isIcon = (Icon == InItemData.Icon);

		return (isName && isType && isBuyPrice && isSellPrice && isMesh && isIcon);
	}
	bool operator !=(const FItemData& InItemData) const
	{
		return !(*this == InItemData);
	}

};
