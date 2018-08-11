// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ParryingState.h"

#include "Monster/MiniBoss/MiniBossAIController.h"
#include "Monster/MiniBoss/MiniBoss.h"

#include "Components/CapsuleComponent.h"

EBTNodeResult::Type UBTTask_ParryingState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			MiniBoss->IsParrying = false;
			MiniBoss->CurrentAnimState = EMiniBossAnimState::Walk;
			MiniBoss->CurrentAttackState = EMiniBossAttackState::Idle;
			MiniBoss->CurrentJumpState = EMiniBossJumpState::Idle;
			MiniBoss->CurrentState = EMiniBossState::Chase;			
		}
	}
	return EBTNodeResult::Succeeded;
}
