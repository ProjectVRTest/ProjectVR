// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBTwoHandWidthStartState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBTwoHandWidthStartState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			float ReverseAttackAnimationWaitTime = 1.2f;			
			AI->BBComponent->SetValueAsFloat(TEXT("ReverseWaitTime"), ReverseAttackAnimationWaitTime);
			MiniBoss->IsAttack = false; //다시 공격할 수 있게 해줌		
		//	MiniBoss->AttackCompleteFlag = true;
			MiniBoss->PlayAnimMontage(MiniBoss->AttackReverseMontage, 1.0f, TEXT("TwoHandIdleReverse"));
		}
	}
	return EBTNodeResult::Succeeded;
}
