// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMComboAttackReadyState.h"
#include "Headers/NormalMonsterAIHeader.h"

EBTNodeResult::Type UBTTask_NMComboAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ANormalMonster* NormalMonster = Cast<ANormalMonster>(AI->GetPawn());

		if (NormalMonster)
		{
			AI->BBComponent->SetValueAsFloat("AttackAnimationWaitTime", 0.3f);
			NormalMonster->CurrentComboAttackState = ENormalMonsterComboAttackState::ComboAttackStart;
		}
	}
	return EBTNodeResult::Succeeded;
}
