// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossAttackReadyState.h"
#include "Headers/BossAIHeader.h"

EBTNodeResult::Type UBTTask_BossAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		float Distance = AI->BBComponent->GetValueAsFloat("Distance");

		ABoss* Boss = Cast<ABoss>(AI->GetPawn());

		if (Boss)
		{
			if (Distance > 400.0f)
			{
				Boss->CurrentLongAttackState = EBossLongAttackState::LongAttackReady;
				Boss->CurrentBattleState = EBossBattleState::LongAttack;
			}
			else
			{

			}
		}
	}

	return EBTNodeResult::Succeeded;
}
