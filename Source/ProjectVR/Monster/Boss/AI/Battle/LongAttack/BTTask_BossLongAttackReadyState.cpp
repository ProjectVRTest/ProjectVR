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
				RandValue = FMath::RandRange(5, 14);				
			}
			else if(Boss->OrbMaxCount >=1)
			{
				RandValue = FMath::RandRange(1, 14);
			}			

			if (RandValue >= 9)
			{
				Boss->CurrentLongAttackState = EBossLongAttackState::DefaultLongAttack;
			}
			else if(RandValue >=5 && RandValue <=8)
			{
				Boss->CurrentWaveAttackState = EBossWaveAttackState::WaveAttackStart;
				Boss->CurrentLongAttackState = EBossLongAttackState::WaveAttack;				
			}
			else
			{
				Boss->CurrentLongAttackState = EBossLongAttackState::OrbCreate;
			}
			
		}		
	}
	
	return EBTNodeResult::Succeeded;
}
