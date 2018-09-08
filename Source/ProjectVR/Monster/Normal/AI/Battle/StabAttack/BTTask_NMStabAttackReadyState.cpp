// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMStabAttackReadyState.h"
#include "Headers/NormalMonsterAIHeader.h"

EBTNodeResult::Type UBTTask_NMStabAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ANormalMonsterAIController* AI = Cast<ANormalMonsterAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ANormalMonster* NormalMonster = Cast<ANormalMonster>(AI->GetPawn());

		if (NormalMonster)
		{
			float Distance = AI->BBComponent->GetValueAsFloat("Distnace");

			if (Distance < 350.0f)
			{
				 //콤보 공격 시작
				NormalMonster->CurrentStabAttackState = ENormalMonsterStabAttackState::ClipplingAttackReady;
			}
			else
			{
				//단타로 끝냄
			}
		}		
	}

	return EBTNodeResult::Succeeded;
}
