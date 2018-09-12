// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMCutAttackReadyState.h"
#include "Headers/NormalMonsterAIHeader.h"

EBTNodeResult::Type UBTTask_NMCutAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AI->BBComponent->SetValueAsFloat("AttackAnimationWaitTime", 0.15f);

		ANormalMonster* NormalMonster = Cast<ANormalMonster>(AI->GetPawn());

		if (NormalMonster)
		{
			NormalMonster->CurrentStabAttackState = ENormalMonsterStabAttackState::ClipplingAttackStart;
		}
	}

	return EBTNodeResult::Succeeded;
}
