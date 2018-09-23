// Fill out your copyright notice in the Description page of Project Settings.

#include "MB_RotateEndState.h"
#include "Headers/MiniBossAIHeader.h"

EBTNodeResult::Type UMB_RotateEndState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AMiniBossAIController* AI = Cast<AMiniBossAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		FRotator AddRotator = AI->BBComponent->GetValueAsRotator("LookAtRotator");
		AMiniBoss* MiniBoss = Cast<AMiniBoss>(AI->GetPawn());

		if (MiniBoss)
		{
			MiniBoss->SetActorRotation(MiniBoss->GetActorRotation() + AddRotator);
		}
	}

	return EBTNodeResult::Succeeded;
}
