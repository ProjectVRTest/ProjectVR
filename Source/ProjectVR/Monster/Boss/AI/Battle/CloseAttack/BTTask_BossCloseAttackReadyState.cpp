// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossCloseAttackReadyState.h"
#include "Headers/BossAIHeader.h"

EBTNodeResult::Type UBTTask_BossCloseAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ABoss* Boss = Cast<ABoss>(AI->GetPawn());

		int RandValue = 0;

		if (Boss)
		{
			RandValue = FMath::RandRange(1, 15);
			//1~5
			if (RandValue > 0 && RandValue < 6)
			{
				Boss->CurrentCloseAttackState = EBossCloseAttackState::RightUpLeftDownAttack;
			}// 6~11
			else if (RandValue > 5 && RandValue < 12)
			{
				Boss->CurrentCloseAttackState = EBossCloseAttackState::ComboAttack;
			}
			else if (RandValue > 11)
			{
				Boss->CurrentCloseAttackState = EBossCloseAttackState::UpDownAttack;
			}

		}
	}

	return EBTNodeResult::Succeeded;
}
