// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemyAIController();
protected:
	virtual void OnPossess(class APawn* InPawn) override;
private:
	UPROPERTY(EditAnywhere, Category = AI)
		class UBehaviorTree* BTree;
	UPROPERTY(EditAnywhere, Category = AI)
		TArray<class AActor*> PatrolPoint;
public:
	int32 CurrentPatrolPoint = 0;
public:
	FORCEINLINE TArray<class AActor*> GetPatrolPoints() const
	{
		return PatrolPoint;
	}
	
};
