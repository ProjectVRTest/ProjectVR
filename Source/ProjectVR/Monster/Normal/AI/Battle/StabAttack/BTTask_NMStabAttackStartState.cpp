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
			if (Distance < 400.0f)
			{
				//콤보 공격 시작
				GLog->Log(FString::Printf(TEXT("3.5m 아래 진입")));
				AI->BBComponent->SetValueAsFloat("AttackAnimationWaitTime", 1.3f);
				AI->BBComponent->SetValueAsFloat("ReverseAnimationWaitTime", 0);
				NormalMonster->CurrentStabAttackState = ENormalMonsterStabAttackState::ClipplingAttackReady;
			}
			else
			{
				NormalMonster->CurrentStabAttackState = ENormalMonsterStabAttackState::Idle;
				NormalMonster->CurrentAnimState = ENormalMonsterAnimState::Walk;
				NormalMonster->CurrentState = ENormalMonsterState::Chase;

				//AI->BBComponent->SetValueAsFloat("AttackAnimationWaitTime", 0);
				//AI->BBComponent->SetValueAsFloat("ReverseAnimationWaitTime", 0);
				//단타로 끝냄 찌르기에서 대기 상태로 돌아가는 몽타주 필요
			}			
		}		
	}
	return EBTNodeResult::Succeeded;
}
