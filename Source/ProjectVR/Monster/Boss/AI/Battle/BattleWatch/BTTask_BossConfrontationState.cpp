// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossConfrontationState.h"
#include "Headers/BossAIHeader.h"

EBTNodeResult::Type UBTTask_BossConfrontationState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());
	
	if (AI)
	{
		ABoss* Boss = Cast<ABoss>(AI->GetPawn());

		if (Boss)
		{
			int RandomPattern = FMath::RandRange(0,1);

			if (RandomPattern == 0)
			{
				Boss->CurrentConfrontationState = EBossConfrontationState::Idle;
				Boss->CurrentBattleWatchState = EBossBattleWatchState::Idle;
				Boss->CurrentBattleState = EBossBattleState::AttackReady;
			}
			else
			{
				Boss->CurrentConfrontationState = EBossConfrontationState::Idle;
				Boss->CurrentBattleWatchState = EBossBattleWatchState::Idle;
				Boss->CurrentLongAttackState = EBossLongAttackState::WaveAttack;
				Boss->CurrentBattleState = EBossBattleState::LongAttack;
			}
		}
	}
	return EBTNodeResult::Succeeded;
}
