// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BossLongAttackReadyState.h"
#include "Headers/BossAIHeader.h"

EBTNodeResult::Type UBTTask_BossLongAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ABossAIController* AI = Cast<ABossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ABoss* Boss = Cast<ABoss>(AI->GetPawn());

		int RandValue=0;

		if (Boss)
		{
			if (Boss->OrbMaxCount < 1)
			{
				RandValue = 5;
			}
			else if(Boss->OrbMaxCount >=1)
			{
				RandValue = FMath::RandRange(1, 10);
			}			

			if (RandValue > 4)
			{
				Boss->CurrentLongAttackState = EBossLongAttackState::DefaultLongAttack;
			}
			else if(RandValue <=4)
			{
				Boss->CurrentLongAttackState = EBossLongAttackState::OrbCreate;
			}
		}		
	}
	
	return EBTNodeResult::Succeeded;
}
