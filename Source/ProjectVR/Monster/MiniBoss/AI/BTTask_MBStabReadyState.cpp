// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBStabReadyState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBStabReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		float WaitTime;
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			WaitTime = 1.2f;
			AI->BBComponent->SetValueAsFloat(TEXT("AttackAnimationWaitTime"), WaitTime);
			MiniBoss->CurrentAttackState = EMiniBossAttackState::StabStart;
		}
	}

	return EBTNodeResult::Succeeded;
}
