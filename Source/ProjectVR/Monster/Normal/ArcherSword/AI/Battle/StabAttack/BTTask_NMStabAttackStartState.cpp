// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMStabAttackStartState.h"
#include "Headers/NormalMonsterAIHeader.h"

EBTNodeResult::Type UBTTask_NMStabAttackStartState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		float Distance = AI->BBComponent->GetValueAsFloat("Distance");

		ANormalMonster* NormalMonster = Cast<ANormalMonster>(AI->GetPawn());

		if (NormalMonster)
		{
			AI->BBComponent->SetValueAsFloat("ReverseAnimationWaitTime", 0.5f); //리버스 애니메이션 재생시간을 설정해주고	
			NormalMonster->PlayAnimMontage(NormalMonster->NMAttackReverseMontage, 1.0f, TEXT("StabReverse"));			
		}		
	}
	return EBTNodeResult::Succeeded;
}
