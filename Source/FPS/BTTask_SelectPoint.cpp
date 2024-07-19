// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SelectPoint.h"

#include "BlackboardKeys.h"
#include "EnemyAIController.h"
#include "PatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SelectPoint::UBTTask_SelectPoint()
{
	NodeName = TEXT("SelectPoint");
}

EBTNodeResult::Type UBTTask_SelectPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (IsValid(AIController) == false) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	//블랙보드에 저장된 정찰 지점을 가져옴
	APatrolPoint* CurrentPoint = Cast<APatrolPoint>(BlackboardComp->GetValueAsObject(BBKeys::PatrolPointKey));

	TArray<AActor*> AvailablePatrolPoints = AIController->GetPatrolPoints();
	//모든 정찰 지점을 가져옴
	APatrolPoint* NextPatrolPoint = nullptr;
	//현재정찰지점이 마지막 정찰지점이 아니라면
	if (AIController->CurrentPatrolPoint != AvailablePatrolPoints.Num() - 1)
	{
		//다음 정찰지점
		NextPatrolPoint = Cast <APatrolPoint>(AvailablePatrolPoints[++AIController->CurrentPatrolPoint]);
	}
	else
	{
		//처음 지점
		NextPatrolPoint = Cast<APatrolPoint>(AvailablePatrolPoints[0]);
		AIController->CurrentPatrolPoint = 0;
	}
	BlackboardComp->SetValueAsObject(BBKeys::PatrolPointKey, NextPatrolPoint);
	return EBTNodeResult::Succeeded; //성공 반환

}
