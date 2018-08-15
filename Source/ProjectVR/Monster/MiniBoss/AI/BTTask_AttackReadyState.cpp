// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_AttackReadyState.h"

#include "Monster/MiniBoss/MiniBossAIController.h"
#include "Monster/MiniBoss/MiniBoss.h"


void UBTTask_AttackReadyState::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_AttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

	}
	return EBTNodeResult::InProgress;
}

void UBTTask_AttackReadyState::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (MiniBoss)
	{
		MiniBoss->AddMovementInput(MiniBoss->GetActorForwardVector(), -1.0f);
		//MiniBoss->CurrentAnimState = EMiniBossAnimState::Walk;
	}
}
