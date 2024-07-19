// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "BlackboardKeys.h"
#include "Gunner.h"
#include "Enemy.h"
#include "PatrolPoint.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAsset(TEXT("BehaviorTree'/Game/Blueprint/Enemy/BT_Enemy.BT_Enemy'"));
	if (BTAsset.Succeeded())
	{
		BTree = BTAsset.Object;
	}

	CurrentPatrolPoint = 0;
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BTree == nullptr) return;
	RunBehaviorTree(BTree);
	Blackboard->InitializeBlackboard(*BTree->BlackboardAsset);
	//���忡 �ִ� ��� PatrpolPoint�� ã��
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPoint::StaticClass(), PatrolPoint);
	Blackboard->SetValueAsObject(BBKeys::PatrolPointKey, PatrolPoint[0]);
	//�����忡 0��° ���������� ����
}
