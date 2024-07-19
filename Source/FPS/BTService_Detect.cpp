// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"

#include "BlackboardKeys.h"
#include "EnemyAIController.h"
#include "Gunner.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
	RandomDeviation = 0.f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* MyPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (IsValid(MyPawn) == false) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(NAME_None, false, MyPawn);

	//DrawDebugSphere(GetWorld(), MyPawn->GetActorLocation(), DetectRadius, 16, FColor::Red, false, 0.2f);

	if (GetWorld()->OverlapMultiByChannel(OverlapResults, MyPawn->GetActorLocation(), FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(DetectRadius), Params))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKeys::TargetKey, nullptr); //저장된 타겟을 null로 초기화
		for (auto Result : OverlapResults)
		{
			if (ACharacter* Other = Cast<ACharacter>(Result.GetActor()))
			{
				if (Other->GetController()->IsPlayerController())
				{
					//플레이어를 블랙보드에 저장
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKeys::TargetKey, Result.GetActor());
					//DrawDebugSphere(GetWorld(), MyPawn->GetActorLocation(), DetectRadius, 16, FColor::Green, false, 0.2f);
					//DrawDebugPoint(GetWorld(), Other->GetActorLocation(), 1.0f, FColor::Blue, false, 0.2f);
					//DrawDebugLine(GetWorld(), MyPawn->GetActorLocation(), Other->GetActorLocation(), FColor::Yellow, false, 0.2f);
					return;
				}
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKeys::TargetKey, nullptr);
	}
}
