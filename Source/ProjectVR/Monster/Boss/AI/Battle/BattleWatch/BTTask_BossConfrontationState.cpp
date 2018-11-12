// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossConfrontationState.h"
#include "Headers/BossAIHeader.h"

EBTNodeResult::Type UBTTask_BossConfrontationState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ABoss* Boss = Cast<ABoss>(AI->GetPawn());

		float Distance = AI->BBComponent->GetValueAsFloat("Distance");

		if (Boss)
		{
			if (Distance > 500.0f)
			{
				Boss->CurrentConfrontationState = EBossConfrontationState::Idle;
				Boss->CurrentBattleWatchState = EBossBattleWatchState::Idle;
				Boss->CurrentWaveAttackState = EBossWaveAttackState::WaveAttackStart;
				Boss->CurrentLongAttackState = EBossLongAttackState::WaveAttack;
				Boss->CurrentBattleState = EBossBattleState::LongAttack;
			}
			else
			{
				Boss->CurrentConfrontationState = EBossConfrontationState::Idle;
				Boss->CurrentBattleWatchState = EBossBattleWatchState::Idle;
				Boss->CurrentBattleState = EBossBattleState::AttackReady;
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
