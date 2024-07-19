// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "Gunner.h"
#include "EnemyAIController.h"
#include "Enemy.h"
#include "BlackboardKeys.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true; //ƽ���
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AEnemyAIController* EnemyController = Cast <AEnemyAIController>(OwnerComp.GetAIOwner());
	if (IsValid(EnemyController) == false) return EBTNodeResult::Failed;
	auto Target = Cast<AGunner>(EnemyController->GetBlackboardComponent()->GetValueAsObject(BBKeys::TargetKey));
	if (IsValid(Target) == false) return EBTNodeResult::Failed;
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (IsValid(Enemy) == false) return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - Enemy->GetActorLocation();
	LookVector.Z = 0.f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Enemy->SetActorRotation(TargetRot);
	Enemy->PlayAttackMontage();
	return EBTNodeResult::InProgress; //���� ���� ���θ� ���߿� ��ȯ
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	AEnemy* Enemy = Cast <AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (IsValid(Enemy) == false) return;
	UAnimInstance* Anim = Cast<UAnimInstance>(Enemy->GetMesh()->GetAnimInstance());
	if (IsValid(Anim) == false) return;
	//���� ��Ÿ�ְ� �����ٸ�
	if (Anim->Montage_GetIsStopped(Enemy->GetCurrentMontage()))
	{
		//���� ��ȯ
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
