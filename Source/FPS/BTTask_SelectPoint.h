// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SelectPoint.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API UBTTask_SelectPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_SelectPoint();
protected:
	virtual EBTNodeResult::Type ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
