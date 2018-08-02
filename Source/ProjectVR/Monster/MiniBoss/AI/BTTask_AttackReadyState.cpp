// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_AttackReadyState.h"

#include "Monster/MiniBoss/MiniBossAIController.h"
#include "Monster/MiniBoss/MiniBoss.h"


EBTNodeResult::Type UBTTask_AttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			int RandomAttack = FMath::RandRange(1, 10);

			if (RandomAttack <= 4)
			{
				MiniBoss->CurrentAttackState = EMonsterAttackState::ParryingAttack;
				MiniBoss->CurrentAnimState = EMonsterAnimState::Wait;
			}
			else if (RandomAttack > 4 && RandomAttack < 8)
			{
				MiniBoss->CurrentAttackState = EMonsterAttackState::DefaultAttack;
				MiniBoss->CurrentAnimState = EMonsterAnimState::Wait;
			}
			else
			{
				MiniBoss->CurrentAttackState = EMonsterAttackState::ParryingAttack;
				MiniBoss->CurrentAnimState = EMonsterAnimState::Wait;
			}

		}
	}
	return EBTNodeResult::Succeeded;
}
