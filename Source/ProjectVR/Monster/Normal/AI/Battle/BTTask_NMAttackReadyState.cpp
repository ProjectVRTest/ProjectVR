// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMAttackReadyState.h"
#include "Headers/NormalMonsterAIHeader.h"

EBTNodeResult::Type UBTTask_NMAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ANormalMonster* NormalMonster = Cast<ANormalMonster>(AI->GetPawn());

		if (NormalMonster)
		{
			int RandomPattern = FMath::RandRange(1, 10);

			if (RandomPattern > 0 && RandomPattern < 7) // 60
			{
				AI->BBComponent->SetValueAsFloat("AttackAnimationWaitTime", 1.8f);
				NormalMonster->CurrentComboAttackState = ENormalMonsterComboAttackState::Idle;
				NormalMonster->CurrentStabAttackState = ENormalMonsterStabAttackState::StabAttackReady;
				NormalMonster->CurrentAttackState = ENormalMonsterAttackState::StabAttack;
			}
			else
			{
				AI->BBComponent->SetValueAsFloat("AttackAnimationWaitTime", 3.7f);
				NormalMonster->CurrentStabAttackState = ENormalMonsterStabAttackState::Idle;
				NormalMonster->CurrentComboAttackState = ENormalMonsterComboAttackState::ComboAttackReady;
				NormalMonster->CurrentAttackState = ENormalMonsterAttackState::ComboAttack;
			}
			NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Attack;
		}
	}

	return EBTNodeResult::Succeeded;
}
