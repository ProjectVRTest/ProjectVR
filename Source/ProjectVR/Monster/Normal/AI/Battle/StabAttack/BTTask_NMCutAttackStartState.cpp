// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMCutAttackStartState.h"
#include "Headers/NormalMonsterAIHeader.h"

EBTNodeResult::Type UBTTask_NMCutAttackStartState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ANormalMonster* NormalMonster = Cast<ANormalMonster>(AI->GetPawn());

		if (NormalMonster)
		{
			NormalMonster->CurrentStabAttackState = ENormalMonsterStabAttackState::Idle;
			NormalMonster->CurrentComboAttackState = ENormalMonsterComboAttackState::Idle;
			NormalMonster->CurrentAttackState = ENormalMonsterAttackState::Idle;
			NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Walk;
			NormalMonster->CurrentState = ENormalMonsterState::Battle;
		}
	}
	return EBTNodeResult::Succeeded;
}
