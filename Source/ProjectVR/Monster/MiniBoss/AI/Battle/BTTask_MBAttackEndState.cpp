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
		float Distance = AI->BBComponent->GetValueAsFloat("Distance");

		if (MiniBoss)
		{
			if (MiniBoss->CurrentShortAttackState != EMiniBossShortAttackState::StabReady)
			{
				int RandomState = FMath::RandRange(1, 10);

				if (RandomState >= 3 && Distance < 300.0f)
				{
					MiniBoss->CurrentState = EMiniBossState::Battle;
					MiniBoss->CurrentAnimState = EMiniBossAnimState::Rotate;
					MiniBoss->CurrentJumpState = EMiniBossJumpState::Idle;
					MiniBoss->CurrentAttackState = EMiniBossAttackState::Idle;
					MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::Idle;
					MiniBoss->CurrentWaveAttackState = EMiniBossWaveAttackState::Idle;
					MiniBoss->CurrentComboAttackState = EMiniBossComboAttackState::Idle;
					MiniBoss->CurrentBackAttackState = EMiniBossBackAttackState::Idle;
				}
				else
				{
					MiniBoss->CurrentState = EMiniBossState::Battle;
					MiniBoss->CurrentAnimState = EMiniBossAnimState::BackWalk;
					MiniBoss->CurrentJumpState = EMiniBossJumpState::Idle;
					MiniBoss->CurrentAttackState = EMiniBossAttackState::Idle;
					MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::Idle;
					MiniBoss->CurrentWaveAttackState = EMiniBossWaveAttackState::Idle;
					MiniBoss->CurrentComboAttackState = EMiniBossComboAttackState::Idle;
					MiniBoss->CurrentBackAttackState = EMiniBossBackAttackState::Idle;
				}
			}
		}
	}

	return EBTNodeResult::Succeeded;
}
