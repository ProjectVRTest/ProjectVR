// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBTwoHandWidthState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBTwoHandWidthState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
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
				AttackAnimationWaitTime = 1.0f;
				ReverseAttackAnimationWaitTime = 0;
				AI->BBComponent->SetValueAsFloat(TEXT("AttackAnimationWaitTime"), AttackAnimationWaitTime);
				AI->BBComponent->SetValueAsFloat(TEXT("ReverseWaitTime"), ReverseAttackAnimationWaitTime);
				MiniBoss->CurrentAttackState = EMiniBossAttackState::StabReady;				
			}
			else
			{
				ReverseAttackAnimationWaitTime = 1.8f;
				AI->BBComponent->SetValueAsFloat(TEXT("ReverseWaitTime"), ReverseAttackAnimationWaitTime);
				MiniBoss->IsAttack = false; //다시 공격할 수 있게 해줌		
				MiniBoss->AttackCompleteFlag = true;
				MiniBoss->PlayAnimMontage(MiniBoss->AttackReverseMontage, 1.0f, TEXT("TwoHandIdleReverse"));
				MiniBoss->CurrentState = EMiniBossState::Chase;
				MiniBoss->CurrentAnimState = EMiniBossAnimState::Walk;
				MiniBoss->CurrentAttackState = EMiniBossAttackState::AttackReady;
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
