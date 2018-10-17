// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossAddAttackStartState.h"
#include "Headers/BossAIHeader.h"

EBTNodeResult::Type UBTTask_BossAddAttackStartState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ABoss* Boss = Cast<ABoss>(AI->GetPawn());

		if (Boss)
		{
			Boss->CurrentBlinkAttackState = EBossBlinkAttackState::InVisible;
		}
	}
	return EBTNodeResult::Succeeded;
}
