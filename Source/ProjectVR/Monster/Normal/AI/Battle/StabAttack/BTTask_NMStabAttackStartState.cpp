// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMStabAttackStartState.h"
#include "Headers/NormalMonsterAIHeader.h"

EBTNodeResult::Type UBTTask_NMStabAttackStartState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		
	}
	return EBTNodeResult::Succeeded;
}
