// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossSetState.h"
#include "Headers/BossAIHeader.h"

EBTNodeResult::Type UBTTask_BossSetState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ABoss* Boss = Cast<ABoss>(AI->GetPawn());

		if (Boss)
		{
			Boss->CurrentState = SetState;
			Boss->CurrentBlinkAttackState = SetBlinkAttackState;
			Boss->CurrentBattleState = SetBattleState;
			Boss->CurrentLongAttackState = SetLongAttackState;
			Boss->CurrentCloseAttackState = SetCloseAttackState;
			Boss->CurrentBattleWatchState = SetBattleWatchState;
			Boss->CurrentConfrontationState = SetConfrontationState;
			Boss->CurrentUltimateAttackState = SetUltimateAttackState;
			Boss->CurrentWaveAttackState = SetWaveAttackState;
			Boss->CurrentUpDownAttackState = SetUpDownAttackState;
		}
	}
	return EBTNodeResult::Succeeded;
}
