// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBAttackReadyState.h"
#include "Headers/MiniBossAIHeader.h"

void UBTTask_MBAttackReadyState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);
}

EBTNodeResult::Type UBTTask_MBAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());
	}
	return EBTNodeResult::Type();
}

void UBTTask_MBAttackReadyState::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{

}
