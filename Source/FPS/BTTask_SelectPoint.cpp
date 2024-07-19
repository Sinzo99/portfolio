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
	//�����忡 ����� ���� ������ ������
	APatrolPoint* CurrentPoint = Cast<APatrolPoint>(BlackboardComp->GetValueAsObject(BBKeys::PatrolPointKey));

	TArray<AActor*> AvailablePatrolPoints = AIController->GetPatrolPoints();
	//��� ���� ������ ������
	APatrolPoint* NextPatrolPoint = nullptr;
	//�������������� ������ ���������� �ƴ϶��
	if (AIController->CurrentPatrolPoint != AvailablePatrolPoints.Num() - 1)
	{
		//���� ��������
		NextPatrolPoint = Cast <APatrolPoint>(AvailablePatrolPoints[++AIController->CurrentPatrolPoint]);
	}
	else
	{
		//ó�� ����
		NextPatrolPoint = Cast<APatrolPoint>(AvailablePatrolPoints[0]);
		AIController->CurrentPatrolPoint = 0;
	}
	BlackboardComp->SetValueAsObject(BBKeys::PatrolPointKey, NextPatrolPoint);
	return EBTNodeResult::Succeeded; //���� ��ȯ

}
