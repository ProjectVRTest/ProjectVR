// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBComboAttackState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBComboAttackState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AI->BBComponent->SetValueAsFloat("AttackAnimationWaitTime", 0.4f);
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			//MiniBoss->AttackCompleteFlag = true;
			MiniBoss->IsAttack = false;
		}
	}
	return EBTNodeResult::Succeeded;
}
