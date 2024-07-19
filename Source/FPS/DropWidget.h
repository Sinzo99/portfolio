// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DropWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UDropWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual bool NativeOnDrop(const FGeometry& InGeomerty, const FDragDropEvent& InDragDropEvent, class UDragDropOperation* InOperation) override;
};
