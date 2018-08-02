// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BattleState.h"
#include "Monster/MiniBoss/MiniBossAIController.h"
#include "Monster/MiniBoss/MiniBoss.h"

EBTNodeResult::Type UBTTask_BattleState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			switch (MiniBoss->CurrentAttackState)
			{
			case EMonsterAttackState::Idle:
				break;
			case EMonsterAttackState::DefaultAttack:
				MiniBoss->CurrentAnimState = EMonsterAnimState::Attack;
				break;
			case EMonsterAttackState::ParryingAttack:
				MiniBoss->CurrentState = EMonsterState::Battle;
				MiniBoss->CurrentAnimState = EMonsterAnimState::Attack;
				break;
			}

			return EBTNodeResult::Succeeded;
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		return EBTNodeResult::Failed;
	}	
}
