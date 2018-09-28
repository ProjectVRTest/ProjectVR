// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBWATwoHandReadyState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBWATwoHandReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			MiniBoss->CurrentWaveAttackState = EMiniBossWaveAttackState::WavebackWalk;
		}
	}
	return EBTNodeResult::Succeeded;
}
