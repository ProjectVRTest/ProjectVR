// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBAttackEndState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBAttackEndState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		AI->BBComponent->SetValueAsFloat(TEXT("AttackAnimationWaitTime"), 0);
		AI->BBComponent->SetValueAsFloat(TEXT("ReverseWaitTime"), 0);

		if (MiniBoss)
		{
			if (MiniBoss->CurrentShortAttackState != EMiniBossShortAttackState::StabReady)
			{
				int RandomState = FMath::RandRange(1, 10);

				if (RandomState >= 5)
				{
					MiniBoss->CurrentState = EMiniBossState::Battle;
					MiniBoss->CurrentAnimState = EMiniBossAnimState::AttackReady;
					MiniBoss->CurrentJumpState = EMiniBossJumpState::Idle;
					MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::Idle;
				}
				else
				{
					MiniBoss->CurrentState = EMiniBossState::Battle;
					MiniBoss->CurrentAnimState = EMiniBossAnimState::BackWalk;
					MiniBoss->CurrentJumpState = EMiniBossJumpState::Idle;
					MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::Idle;
				}
			}
		}
	}

	return EBTNodeResult::Succeeded;
}
