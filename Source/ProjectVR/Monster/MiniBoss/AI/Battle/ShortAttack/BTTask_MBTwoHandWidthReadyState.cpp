// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBTwoHandWidthReadyState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBTwoHandWidthReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		float AttackAnimationWaitTime;
		float ReverseAttackAnimationWaitTime;
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			float Distance = AI->BBComponent->GetValueAsFloat("Distance");

			if (Distance < 350.0f)
			{
				AttackAnimationWaitTime = 0.4f;
				ReverseAttackAnimationWaitTime = 0;
				AI->BBComponent->SetValueAsFloat(TEXT("ReverseWaitTime"), ReverseAttackAnimationWaitTime);
				MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::StabReady;
			}
			else
			{
				AttackAnimationWaitTime = 0.1f;
				MiniBoss->CurrentShortAttackState = EMiniBossShortAttackState::TwoHandWidthStart;
			}

			AI->BBComponent->SetValueAsFloat(TEXT("AttackAnimationWaitTime"), AttackAnimationWaitTime);
		}
	}
	return EBTNodeResult::Succeeded;
}
