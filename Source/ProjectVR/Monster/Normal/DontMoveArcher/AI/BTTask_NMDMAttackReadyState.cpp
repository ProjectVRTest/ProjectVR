// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_NMDMAttackReadyState.h"
#include "Headers/DontMoveArcherAIHeader.h"

EBTNodeResult::Type UBTTask_NMDMAttackReadyState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ADontMoveArcherAIController* AI = Cast<ADontMoveArcherAIController>(OwnerComp.GetAIOwner());

	if (AI)
	{
		ADontMoveArcher* DontMoveArcher = Cast<ADontMoveArcher>(AI->GetPawn());

		if (DontMoveArcher)
		{
			DontMoveArcher->CurrentArcherAttackState = EDontMoveArcherArcherAttackState::ArcherAttackReady;
			DontMoveArcher->CurrentAttackState = EDontMoveArcherAttackState::BowAttack;
			DontMoveArcher->CurrentAnimState = EDontMoveArcherAnimState::Attack;
		}
	}

	return EBTNodeResult::Succeeded;
}
