// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"

#include "Gunner.h"
#include "EnemyAIController.h"
#include "Enemy.h"
#include "BlackboardKeys.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true; //틱사용
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
	return EBTNodeResult::InProgress; //성공 실패 여부를 나중에 반환
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	AEnemy* Enemy = Cast <AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (IsValid(Enemy) == false) return;
	UAnimInstance* Anim = Cast<UAnimInstance>(Enemy->GetMesh()->GetAnimInstance());
	if (IsValid(Anim) == false) return;
	//현재 몽타주가 끝났다면
	if (Anim->Montage_GetIsStopped(Enemy->GetCurrentMontage()))
	{
		//성공 반환
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
