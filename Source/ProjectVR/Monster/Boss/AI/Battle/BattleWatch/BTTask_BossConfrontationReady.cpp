// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossConfrontationReady.h"
#include "Headers/BossAIHeader.h"

EBTNodeResult::Type UBTTask_BossConfrontationReady::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ABoss* Boss = Cast<ABoss>(AI->GetPawn());

		if (Boss)
		{
			int RandomPattern = FMath::RandRange(0, 1);

			if (RandomPattern == 0)
			{
				Boss->CurrentConfrontationState = EBossConfrontationState::LeftWalk;
			}
			else
			{
				Boss->CurrentConfrontationState = EBossConfrontationState::RightWalk;
			}
		}
	}

	return EBTNodeResult::Succeeded;
}
