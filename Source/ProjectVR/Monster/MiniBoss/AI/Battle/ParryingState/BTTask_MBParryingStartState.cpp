// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MBParryingStartState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UBTTask_MBParryingStartState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			GLog->Log(FString::Printf(TEXT("패링 시작 진입")));
			MiniBoss->CurrentParryingState = EMiniBossParryingState::ParryingLoop;
		}
	}
	return EBTNodeResult::Succeeded;
}
