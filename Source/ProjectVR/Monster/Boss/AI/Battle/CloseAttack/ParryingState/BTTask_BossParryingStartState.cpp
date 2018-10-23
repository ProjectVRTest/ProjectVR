// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossParryingStartState.h"
#include "Headers/BossAIHeader.h"

EBTNodeResult::Type UBTTask_BossParryingStartState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ABoss* Boss = Cast<ABoss>(AI->GetPawn());

		if (Boss)
		{
			Boss->CurrentParryingState = EBossParryingState::ParryingLoop;
		}
	}

	return EBTNodeResult::Succeeded;
}
