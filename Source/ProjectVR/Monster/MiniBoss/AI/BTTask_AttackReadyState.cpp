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
				MiniBoss->CurrentAttackState = EMiniBossAttackState::ParryingAttack;
				MiniBoss->CurrentAnimState = EMiniBossAnimState::Wait;
			}
			else if (RandomAttack > 4 && RandomAttack < 8)
			{
				MiniBoss->CurrentAttackState = EMiniBossAttackState::DefaultAttack;
				MiniBoss->CurrentAnimState = EMiniBossAnimState::Wait;
			}
			else
			{
				MiniBoss->CurrentAttackState = EMiniBossAttackState::ParryingAttack;
				MiniBoss->CurrentAnimState = EMiniBossAnimState::Wait;
			}

		}
	}
	return EBTNodeResult::Succeeded;
}
