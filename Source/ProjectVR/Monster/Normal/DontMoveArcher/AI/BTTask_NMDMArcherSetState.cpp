// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMDMArcherSetState.h"
#include "Headers/DontMoveArcherAIHeader.h"

EBTNodeResult::Type UBTTask_NMDMArcherSetState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ADontMoveArcherAIController* AI = Cast<ADontMoveArcherAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ADontMoveArcher* DontMoveArcher = Cast<ADontMoveArcher>(AI->GetPawn());

		if (DontMoveArcher)
		{
			DontMoveArcher->CurrentState = SetState;
			DontMoveArcher->CurrentAnimState = SetAnimState;
			DontMoveArcher->CurrentAttackState = SetAttackState;
			DontMoveArcher->CurrentArcherAttackState = SetArcherAttackState;
		}
	}

	return EBTNodeResult::Succeeded;
}
