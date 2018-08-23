// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBJumpAttackReadyState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBJumpAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			float Distance = AI->BBComponent->GetValueAsFloat("Distance");
			MiniBoss->CurrentAnimState = EMiniBossAnimState::JumpAttack;
			MiniBoss->CurrentJumpState = EMiniBossJumpState::JumpStart;	      
		}
	}

	return EBTNodeResult::Succeeded;
}

void UBTTask_MBJumpAttackReadyState::JumpRunCheck()
{
	if (MiniBoss)
	{
		if (MiniBoss->CurrentState == EMiniBossState::Chase)
		{
			if (MiniBoss->CurrentAnimState == EMiniBossAnimState::Walk)
			{
				MiniBoss->CurrentAnimState = EMiniBossAnimState::DashAttack;
			}
			else
			{

			}
		}
	}
}
