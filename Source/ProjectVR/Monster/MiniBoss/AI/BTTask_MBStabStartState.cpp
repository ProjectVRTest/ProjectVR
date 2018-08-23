// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBStabStartState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBStabStartState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		float WaitTime;
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			WaitTime = 1.0f;
			AI->BBComponent->SetValueAsFloat(TEXT("AttackAnimationWaitTime"), WaitTime);
			MiniBoss->PlayAnimMontage(MiniBoss->AttackReverseMontage, 1.0f, TEXT("StabIdleReverse"));
			MiniBoss->AttackCompleteFlag = true;
			MiniBoss->IsAttack = false;			
		}
	}
	return EBTNodeResult::Succeeded;
}
