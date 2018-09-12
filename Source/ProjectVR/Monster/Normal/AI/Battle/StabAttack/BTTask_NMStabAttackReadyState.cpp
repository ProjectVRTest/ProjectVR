// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMStabAttackReadyState.h"
#include "Headers/NormalMonsterAIHeader.h"

EBTNodeResult::Type UBTTask_NMStabAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AI->BBComponent->SetValueAsFloat("AttackAnimationWaitTime", 0.2f);

		ANormalMonster* NormalMonster = Cast<ANormalMonster>(AI->GetPawn());

		if (NormalMonster)
		{
			NormalMonster->CurrentStabAttackState = ENormalMonsterStabAttackState::StabAttackStart;		
		}		
	}

	return EBTNodeResult::Succeeded;
}