// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMSetState.h"
#include "Headers/NormalMonsterAIHeader.h"

EBTNodeResult::Type UBTTask_NMSetState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ANormalMonster* NormalMonster = Cast<ANormalMonster>(AI->GetPawn());

		if (NormalMonster)
		{
			NormalMonster->CurrentState = SetState;
			NormalMonster->CurrentAnimState = SetAnimState;
			NormalMonster->CurrentIdleState = SetIdleState;
			NormalMonster->CurrentAttackState = SetAttackState;
			NormalMonster->CurrentStabAttackState = SetStabAttackState;
			NormalMonster->CurrentComboAttackState = SetComboAttackState;
		}
	}
	return EBTNodeResult::Succeeded;
}
